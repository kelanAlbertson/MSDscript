//
// Created by Kelan Albertson on 1/16/22.
//

#include "NumExpr.h"
#include "VarExpr.h"
#include "catch.h"
#include "NumVal.h"
#include <sstream>

NumExpr::NumExpr(int rep) {
    this->rep_ = rep;
}

bool NumExpr::equals(Expr* other) {
    NumExpr* n = dynamic_cast<NumExpr*>(other);
    if (n == nullptr) {
        return false;
    }
    else {
        return (this->rep_ == n->rep_);
    }
}

Val * NumExpr::interp() {
    return new NumVal(this->rep_);
}

bool NumExpr::has_variable() {
    return false;
}

Expr* NumExpr::subst(std::string variableName, Expr *replacement) {
    return this;
}

void NumExpr::print(std::ostream &out) {
    out << this->rep_;
}

void NumExpr::pretty_print_at(std::ostream &out, precedence_t prec, bool let_parentheses, std::streampos &last_new_line_pos) {
    out << this->rep_;
}

/**
 *************************   TESTS   **************************
 **/

TEST_CASE("NumExpr equals() tests") {
    CHECK((new NumExpr(0))->equals(new NumExpr(0)) == true);
    CHECK((new NumExpr(8))->equals(new NumExpr(8)) == true);
    CHECK((new NumExpr(8))->equals(new NumExpr(-1)) == false);
    CHECK((new NumExpr(8))->equals(new VarExpr("8")) == false);
}

TEST_CASE("NumExpr interp() tests") {
    CHECK((new NumExpr(0))->interp() == 0);
    CHECK((new NumExpr(1))->interp() == 1);
    CHECK((new NumExpr(99))->interp() == 99);
    CHECK((new NumExpr(-1))->interp() == -1);
}

TEST_CASE("NumExpr has_variable() tests") {
    CHECK((new NumExpr(0))->has_variable() == false);
    CHECK((new NumExpr(-11))->has_variable() == false);
}

TEST_CASE("NumExpr subst() tests") {
    CHECK((new NumExpr(1))->subst("x", new VarExpr("y"))->equals(new NumExpr(1)));
}

TEST_CASE("NumExpr print()/to_string() tests") {
    CHECK((new NumExpr(1))->to_string() == "1");
}

TEST_CASE("NumExpr pretty_print() tests") {
    std::stringstream out("");
    (new NumExpr(4))->pretty_print(out);
    CHECK(out.str() == "4");
}