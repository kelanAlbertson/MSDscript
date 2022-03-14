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


LetExpr::LetExpr(PTR(VarExpr) lhs, PTR(Expr) rhs, PTR(Expr) body) {
    this->lhs_ = lhs;
    this->rhs_ = rhs;
    this->body_ = body;
}

bool LetExpr::equals(PTR(Expr) other) {
    PTR(LetExpr) l = CAST(LetExpr)(other);
    if (l == nullptr) {
        return false;
    }
    else {
        return (this->lhs_->equals(l->lhs_) && this->rhs_->equals(l->rhs_) && this->body_->equals(l->body_));
    }
}

PTR(Val) LetExpr::interp() {
    PTR(Val) rhs_val = this->rhs_->interp();
    return this->body_->subst(this->lhs_->name_, rhs_val->to_expr())->interp();
}

//bool LetExpr::has_variable() {
//    return (this->rhs_->has_variable() || this->body_->has_variable());
//}

PTR(Expr)LetExpr::subst(std::string variableName, PTR(Expr) replacement) {
    // rules from lecture videos:
    // if bind same variable then do not substitute in body
    // if bind different variable then do substitute in body
    // always substitute in rhs
    if (this->lhs_->name_ == variableName) {
        return NEW(LetExpr)(this->lhs_, this->rhs_->subst(variableName, replacement), this->body_);
    }
    else {
        return NEW(LetExpr)(this->lhs_,
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

void LetExpr::pretty_print_at(std::ostream &out, Expr::precedence_t prec, bool keyword_parentheses, std::streampos &last_new_line_pos) {
    if (keyword_parentheses) {
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

    if (keyword_parentheses) {
        out << ")";
    }
}

/**
 *************************   TESTS   **************************
 **/

TEST_CASE("LetExpr equals() tests") {
    CHECK((NEW(LetExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1), NEW(NumExpr)(0)))->equals(NEW(LetExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1), NEW(NumExpr)(0))) == true);
    CHECK((NEW(LetExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1), NEW(NumExpr)(0)))->equals(NEW(LetExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(13), NEW(NumExpr)(-2))) == false);
    CHECK((NEW(LetExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr) ("x"), NEW(NumExpr)(1))))
            ->equals(NEW(LetExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr) ("x"), NEW(NumExpr)(1)))) == true);
    CHECK((NEW(LetExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr) ("x"), NEW(NumExpr)(1))))
            ->equals(NEW(LetExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr) ("a"), NEW(NumExpr)(1)))) == false);
    CHECK((NEW(LetExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1), NEW(NumExpr)(0)))->equals(NEW(VarExpr)("x")) == false);
}

TEST_CASE("LetExpr interp() tests") {
    CHECK((NEW(LetExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))
            ->interp()->equals(NEW(NumVal)(6)));
    CHECK((NEW(LetExpr)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(NumExpr)(5), NEW(NumExpr)(2)), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))
            ->interp()->equals(NEW(NumVal)(8)));
    CHECK((NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5), NEW(VarExpr)("x"))), NEW(NumExpr)(1)))
            ->interp()->equals(NEW(NumVal)(26)));
    CHECK((NEW(LetExpr)(NEW(VarExpr)("x"), NEW(LetExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(6), NEW(MultExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))
            ->interp()->equals(NEW(NumVal)(13)));
}

//TEST_CASE("LetExpr has_variable() tests") {
//    CHECK((new LetExpr(new VarExpr("x"), new NumExpr(1), new AddExpr(new VarExpr("x"), new NumExpr(1))))->has_variable() == true);
//    CHECK((new LetExpr(new VarExpr("x"), new AddExpr(new VarExpr("y"), new NumExpr(1)), new NumExpr(1)))->has_variable() == true);
//    CHECK((new LetExpr(new VarExpr("x"), new NumExpr(1), new NumExpr(0)))->has_variable() == false);
//}

TEST_CASE("LetExpr subst() tests") {
    CHECK((NEW(LetExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(6), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))->subst("x", NEW(NumExpr)(5))
                ->equals(NEW(LetExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(6), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))));
    CHECK((NEW(LetExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(6), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))->subst("x", NEW(NumExpr)(5))
                  ->equals(NEW(LetExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(6), NEW(AddExpr)(NEW(NumExpr)(5), NEW(NumExpr)(1)))));
    CHECK((NEW(LetExpr)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2)), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))->subst("x", NEW(NumExpr)(5))
                  ->equals(NEW(LetExpr)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(NumExpr)(5), NEW(NumExpr)(2)), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))));
    CHECK((NEW(LetExpr)(NEW(VarExpr)("x"), NEW(LetExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(6), NEW(MultExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))));
}

TEST_CASE("LetExpr print()/to_string() tests") {
    CHECK((NEW(LetExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(3), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))))->to_string() == "(_let y=3 _in (y+2))");
    CHECK((NEW(LetExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5), NEW(LetExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(3), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2)))))
            ->to_string() == "(_let x=5 _in (_let y=3 _in (y+2)))");
    CHECK((NEW(LetExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5), NEW(AddExpr)(NEW(LetExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(3), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))), NEW(VarExpr)("x"))))
            ->to_string() == "(_let x=5 _in ((_let y=3 _in (y+2))+x))");
}

TEST_CASE("LetExpr pretty_print() tests") {
    std::stringstream out("");
    (NEW(LetExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))->pretty_print(out);
    CHECK(out.str() == "_let x = 5\n"
                       "_in  x + 1");
    out.str(std::string());
    (NEW(AddExpr)(NEW(LetExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5), NEW(VarExpr)("x")), NEW(NumExpr)(1)))->pretty_print(out);
    CHECK(out.str() == "(_let x = 5\n"
                       " _in  x) + 1");
    out.str(std::string());
    (NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))))->pretty_print(out);
    CHECK(out.str() == "5 * _let x = 5\n"
                       "    _in  x + 1");
    out.str(std::string());
    (NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5), NEW(VarExpr)("x"))), NEW(NumExpr)(1)))->pretty_print(out);
    CHECK(out.str() == "5 * (_let x = 5\n"
                       "     _in  x) + 1");
    out.str(std::string());
    (NEW(LetExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5), NEW(AddExpr)(NEW(LetExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(3), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))), NEW(VarExpr)("x"))))
            ->pretty_print(out);
    CHECK(out.str() == "_let x = 5\n"
                       "_in  (_let y = 3\n"
                       "      _in  y + 2) + x");
    out.str(std::string());
    (NEW(LetExpr)(NEW(VarExpr)("x"), NEW(LetExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(6), NEW(MultExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))->pretty_print(out);
    CHECK(out.str() == "_let x = _let y = 6\n"
                       "         _in  y * 2\n"
                       "_in  x + 1");
}