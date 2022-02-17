//
// Created by Kelan Albertson on 1/16/22.
//

#include "MultExpr.h"
#include "AddExpr.h"
#include "NumExpr.h"
#include "NumVal.h"
#include "VarExpr.h"
#include "catch.h"
#include <sstream>

MultExpr::MultExpr(Expr *lhs, Expr *rhs) {
    this->lhs_ = lhs;
    this->rhs_ = rhs;
}

bool MultExpr::equals(Expr *other) {
    MultExpr* m = dynamic_cast<MultExpr*>(other);
    if (m == nullptr) {
        return false;
    }
    else {
        return (this->lhs_->equals(m->lhs_) && this->rhs_->equals(m->rhs_));
    }
}

Val * MultExpr::interp() {
    return (this->lhs_->interp()->multiply_by(this->rhs_->interp()));
}

bool MultExpr::has_variable() {
    return (this->lhs_->has_variable() || this->rhs_->has_variable());
}

Expr* MultExpr::subst(std::string variableName, Expr *replacement) {
    return new MultExpr(this->lhs_->subst(variableName, replacement),
                        this->rhs_->subst(variableName, replacement));
}

void MultExpr::print(std::ostream &out) {
    out << "(";
    this->lhs_->print(out);
    out << "*";
    this->rhs_->print(out);
    out << ")";
}

void MultExpr::pretty_print_at(std::ostream &out, precedence_t prec, bool let_parentheses, std::streampos &last_new_line_pos) {
    if (prec == prec_mult) {
        out << "(";
    }

    this->lhs_->pretty_print_at(out, prec_mult, true, last_new_line_pos);
    out << " * ";
    this->rhs_->pretty_print_at(out, prec_add, let_parentheses, last_new_line_pos);

    if (prec == prec_mult) {
        out << ")";
    }
}

/**
 *************************   TESTS   **************************
 **/

TEST_CASE("MultExpr equals() tests") {
    CHECK((new MultExpr(new NumExpr(-1), new NumExpr(0)))->equals(new MultExpr(new NumExpr(-1), new NumExpr(0))) == true);
    CHECK((new MultExpr(new NumExpr(-1), new NumExpr(0)))->equals(new MultExpr(new NumExpr(0), new NumExpr(-1))) == false);
    CHECK((new MultExpr(new NumExpr(-1), new NumExpr(0)))->equals(new MultExpr(new NumExpr(100), new NumExpr(0))) == false);
    CHECK((new MultExpr(new NumExpr(-1), new NumExpr(0)))->equals(new AddExpr(new NumExpr(-1), new NumExpr(0))) == false);
}

TEST_CASE("MultExpr interp() tests") {
    CHECK((new MultExpr(new NumExpr(0), new NumExpr(0)))->interp() == 0);
    CHECK((new MultExpr(new NumExpr(0), new NumExpr(1)))->interp() == 0);
    CHECK((new MultExpr(new NumExpr(-1), new NumExpr(10)))->interp() == -10);
    CHECK((new MultExpr(new NumExpr(-5), new NumExpr(-5)))->interp() == 25);
}

TEST_CASE("MultExpr has_variable() tests") {
    CHECK((new MultExpr(new NumExpr(0), new NumExpr(1)))->has_variable() == false);
    CHECK((new MultExpr(new VarExpr("test"), new NumExpr(1)))->has_variable() == true);
    CHECK((new MultExpr(new VarExpr("test"), new VarExpr("test")))->has_variable() == true);
}

TEST_CASE("MultExpr subst() tests") {
    CHECK((new MultExpr(new VarExpr("x"), new VarExpr("x")))->subst("x", new VarExpr("y"))
            ->equals(new MultExpr(new VarExpr("y"), new VarExpr("y"))));
    CHECK((new MultExpr(new VarExpr("a"), new VarExpr("x")))->subst("x", new VarExpr("y"))
            ->equals(new MultExpr(new VarExpr("a"), new VarExpr("y"))));
}

TEST_CASE("MultExpr print() tests") {
    CHECK((new MultExpr(new NumExpr(1), new NumExpr(2)))->to_string() == "(1*2)");
    CHECK((new MultExpr(new AddExpr(new VarExpr("test"), new NumExpr(0)), new NumExpr(1)))->to_string() == "((test+0)*1)");
}

TEST_CASE("MultExpr pretty_print() tests") {
    std::stringstream out("");
    (new MultExpr(new NumExpr(1), new NumExpr(2)))->pretty_print(out);
    CHECK(out.str() == "1 * 2");
    out.str(std::string());
    (new MultExpr(new MultExpr(new NumExpr(2), new NumExpr(3)), new NumExpr(4)))->pretty_print(out);
    CHECK(out.str() == "(2 * 3) * 4");
    out.str(std::string());
    (new MultExpr(new NumExpr(2), new MultExpr(new NumExpr(3), new NumExpr(4))))->pretty_print(out);
    CHECK(out.str() == "2 * 3 * 4");
    out.str(std::string());
    (new MultExpr(new AddExpr(new NumExpr(1), new NumExpr(2)), new AddExpr(new NumExpr(3), new NumExpr(4))))
            ->pretty_print(out);
    CHECK(out.str() == "(1 + 2) * (3 + 4)");
}