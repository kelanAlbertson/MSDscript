//
// Created by Kelan Albertson on 1/28/22.
//

#include "LetExpr.h"
#include "AddExpr.h"
#include "MultExpr.h"
#include "NumExpr.h"
#include "NumVal.h"
#include "VarExpr.h"
#include "catch.h"
#include <sstream>
#include <iostream>


LetExpr::LetExpr(VarExpr* lhs, Expr* rhs, Expr* body) {
    this->lhs_ = lhs;
    this->rhs_ = rhs;
    this->body_ = body;
}

bool LetExpr::equals(Expr* other) {
    LetExpr* l = dynamic_cast<LetExpr*>(other);
    if (l == nullptr) {
        return false;
    }
    else {
        return (this->lhs_->equals(l->lhs_) && this->rhs_->equals(l->rhs_) && this->body_->equals(l->body_));
    }
}

Val * LetExpr::interp() {
    Val* rhs_val = this->rhs_->interp();
    return this->body_->subst(this->lhs_->name_, rhs_val->to_expr())->interp();
}

bool LetExpr::has_variable() {
    return (this->rhs_->has_variable() || this->body_->has_variable());
}

Expr *LetExpr::subst(std::string variableName, Expr *replacement) {
    // rules from lecture videos:
    // if bind same variable then do not substitute in body
    // if bind different variable then do substitute in body
    // always substitute in rhs
    if (this->lhs_->name_ == variableName) {
        return new LetExpr(this->lhs_, this->rhs_->subst(variableName, replacement), this->body_);
    }
    else {
        return new LetExpr(this->lhs_,
                           this->rhs_->subst(variableName, replacement),
                           this->body_->subst(variableName, replacement));
    }
}

void LetExpr::print(std::ostream &out) {
    out << "(_let ";
    this->lhs_->print(out);
    out << "=";
    this->rhs_->print(out);
    out << " _in ";
    this->body_->print(out);
    out << ")";
}

void LetExpr::pretty_print_at(std::ostream &out, Expr::precedence_t prec, bool let_parentheses, std::streampos &last_new_line_pos) {
    if (let_parentheses) {
        out << "(";
    }

    int let_indent = out.tellp() - last_new_line_pos;
    out << "_let " + this->lhs_->name_ + " = ";
    this->rhs_->pretty_print_at(out, prec_none, false, last_new_line_pos);
    out << "\n";
    last_new_line_pos = out.tellp();

    for (int i = 0; i < let_indent; ++i) {
        out << " ";
    }

    out << "_in  ";
    this->body_->pretty_print_at(out, prec_none, false, last_new_line_pos);

    if (let_parentheses) {
        out << ")";
    }
}

/**
 *************************   TESTS   **************************
 **/

TEST_CASE("LetExpr equals() tests") {
    CHECK((new LetExpr(new VarExpr("x"), new NumExpr(1), new NumExpr(0)))->equals(new LetExpr(new VarExpr("x"), new NumExpr(1), new NumExpr(0))) == true);
    CHECK((new LetExpr(new VarExpr("x"), new NumExpr(1), new NumExpr(0)))->equals(new LetExpr(new VarExpr("y"), new NumExpr(13), new NumExpr(-2))) == false);
    CHECK((new LetExpr(new VarExpr("x"), new NumExpr(5), new AddExpr(new VarExpr ("x"), new NumExpr(1))))
            ->equals(new LetExpr(new VarExpr("x"), new NumExpr(5), new AddExpr(new VarExpr ("x"), new NumExpr(1)))) == true);
    CHECK((new LetExpr(new VarExpr("x"), new NumExpr(5), new AddExpr(new VarExpr ("x"), new NumExpr(1))))
            ->equals(new LetExpr(new VarExpr("x"), new NumExpr(5), new AddExpr(new VarExpr ("a"), new NumExpr(1)))) == false);
    CHECK((new LetExpr(new VarExpr("x"), new NumExpr(1), new NumExpr(0)))->equals(new VarExpr("x")) == false);
}

TEST_CASE("LetExpr interp() tests") {
    CHECK((new LetExpr(new VarExpr("x"), new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1))))
            ->interp()->equals(new NumVal(6)));
    CHECK((new LetExpr(new VarExpr("x"), new AddExpr(new NumExpr(5), new NumExpr(2)), new AddExpr(new VarExpr("x"), new NumExpr(1))))
            ->interp()->equals(new NumVal(8)));
    CHECK((new AddExpr(new MultExpr(new NumExpr(5), new LetExpr(new VarExpr("x"), new NumExpr(5), new VarExpr("x"))), new NumExpr(1)))
            ->interp()->equals(new NumVal(26)));
    CHECK((new LetExpr(new VarExpr("x"), new LetExpr(new VarExpr("y"), new NumExpr(6), new MultExpr(new VarExpr("y"), new NumExpr(2))), new AddExpr(new VarExpr("x"), new NumExpr(1))))
            ->interp()->equals(new NumVal(13)));
}

TEST_CASE("LetExpr has_variable() tests") {
    CHECK((new LetExpr(new VarExpr("x"), new NumExpr(1), new AddExpr(new VarExpr("x"), new NumExpr(1))))->has_variable() == true);
    CHECK((new LetExpr(new VarExpr("x"), new AddExpr(new VarExpr("y"), new NumExpr(1)), new NumExpr(1)))->has_variable() == true);
    CHECK((new LetExpr(new VarExpr("x"), new NumExpr(1), new NumExpr(0)))->has_variable() == false);
}

TEST_CASE("LetExpr subst() tests") {
    CHECK((new LetExpr(new VarExpr("x"), new NumExpr(6), new AddExpr(new VarExpr("x"), new NumExpr(1))))->subst("x", new NumExpr(5))
                ->equals(new LetExpr(new VarExpr("x"), new NumExpr(6), new AddExpr(new VarExpr("x"), new NumExpr(1)))));
    CHECK((new LetExpr(new VarExpr("y"), new NumExpr(6), new AddExpr(new VarExpr("x"), new NumExpr(1))))->subst("x", new NumExpr(5))
                  ->equals(new LetExpr(new VarExpr("y"), new NumExpr(6), new AddExpr(new NumExpr(5), new NumExpr(1)))));
    CHECK((new LetExpr(new VarExpr("x"), new AddExpr(new VarExpr("x"), new NumExpr(2)), new AddExpr(new VarExpr("x"), new NumExpr(1))))->subst("x", new NumExpr(5))
                  ->equals(new LetExpr(new VarExpr("x"), new AddExpr(new NumExpr(5), new NumExpr(2)), new AddExpr(new VarExpr("x"), new NumExpr(1)))));
    CHECK((new LetExpr(new VarExpr("x"), new LetExpr(new VarExpr("y"), new NumExpr(6), new MultExpr(new VarExpr("y"), new NumExpr(2))), new AddExpr(new VarExpr("x"), new NumExpr(1)))));
}

TEST_CASE("LetExpr print()/to_string() tests") {
    CHECK((new LetExpr(new VarExpr("y"), new NumExpr(3), new AddExpr(new VarExpr("y"), new NumExpr(2))))->to_string() == "(_let y=3 _in (y+2))");
    CHECK((new LetExpr(new VarExpr("x"), new NumExpr(5), new LetExpr(new VarExpr("y"), new NumExpr(3), new AddExpr(new VarExpr("y"), new NumExpr(2)))))
            ->to_string() == "(_let x=5 _in (_let y=3 _in (y+2)))");
    CHECK((new LetExpr(new VarExpr("x"), new NumExpr(5), new AddExpr(new LetExpr(new VarExpr("y"), new NumExpr(3), new AddExpr(new VarExpr("y"), new NumExpr(2))), new VarExpr("x"))))
            ->to_string() == "(_let x=5 _in ((_let y=3 _in (y+2))+x))");
}

TEST_CASE("LetExpr pretty_print() tests") {
    std::stringstream out("");
    (new LetExpr(new VarExpr("x"), new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1))))->pretty_print(out);
    CHECK(out.str() == "_let x = 5\n"
                       "_in  x + 1");
    out.str(std::string());
    (new AddExpr(new LetExpr(new VarExpr("x"), new NumExpr(5), new VarExpr("x")), new NumExpr(1)))->pretty_print(out);
    CHECK(out.str() == "(_let x = 5\n"
                       " _in  x) + 1");
    out.str(std::string());
    (new MultExpr(new NumExpr(5), new LetExpr(new VarExpr("x"), new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1)))))->pretty_print(out);
    CHECK(out.str() == "5 * _let x = 5\n"
                       "    _in  x + 1");
    out.str(std::string());
    (new AddExpr(new MultExpr(new NumExpr(5), new LetExpr(new VarExpr("x"), new NumExpr(5), new VarExpr("x"))), new NumExpr(1)))->pretty_print(out);
    CHECK(out.str() == "5 * (_let x = 5\n"
                       "     _in  x) + 1");
    out.str(std::string());
    (new LetExpr(new VarExpr("x"), new NumExpr(5), new AddExpr(new LetExpr(new VarExpr("y"), new NumExpr(3), new AddExpr(new VarExpr("y"), new NumExpr(2))), new VarExpr("x"))))
            ->pretty_print(out);
    CHECK(out.str() == "_let x = 5\n"
                       "_in  (_let y = 3\n"
                       "      _in  y + 2) + x");
    out.str(std::string());
    (new LetExpr(new VarExpr("x"), new LetExpr(new VarExpr("y"), new NumExpr(6), new MultExpr(new VarExpr("y"), new NumExpr(2))), new AddExpr(new VarExpr("x"), new NumExpr(1))))->pretty_print(out);
    CHECK(out.str() == "_let x = _let y = 6\n"
                       "         _in  y * 2\n"
                       "_in  x + 1");
}