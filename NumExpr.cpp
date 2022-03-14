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

bool NumExpr::equals(PTR(Expr) other) {
    PTR(NumExpr) n = CAST(NumExpr)(other);
    if (n == nullptr) {
        return false;
    }
    else {
        return (this->rep_ == n->rep_);
    }
}

PTR(Val) NumExpr::interp() {
    return NEW(NumVal)(this->rep_);
}

//bool NumExpr::has_variable() {
//    return false;
//}

PTR(Expr) NumExpr::subst(std::string variableName, PTR(Expr) replacement) {
    return THIS;
}

void NumExpr::print(std::ostream &out) {
    out << this->rep_;
}

void NumExpr::pretty_print_at(std::ostream &out, precedence_t prec, bool keyword_parentheses, std::streampos &last_new_line_pos) {
    out << this->rep_;
}

/**
 *************************   TESTS   **************************
 **/

TEST_CASE("NumExpr equals() tests") {
    CHECK((NEW(NumExpr)(0))->equals(NEW(NumExpr)(0)) == true);
    CHECK((NEW(NumExpr)(8))->equals(NEW(NumExpr)(8)) == true);
    CHECK((NEW(NumExpr)(8))->equals(NEW(NumExpr)(-1)) == false);
    CHECK((NEW(NumExpr)(8))->equals(NEW(VarExpr)("8")) == false);
}

TEST_CASE("NumExpr interp() tests") {
    CHECK((NEW(NumExpr)(0))->interp()->equals(NEW(NumVal)(0)));
    CHECK((NEW(NumExpr)(1))->interp()->equals(NEW(NumVal)(1)));
    CHECK((NEW(NumExpr)(99))->interp()->equals(NEW(NumVal)(99)));
    CHECK((NEW(NumExpr)(-1))->interp()->equals(NEW(NumVal)(-1)));
}

//TEST_CASE("NumExpr has_variable() tests") {
//    CHECK((new NumExpr(0))->has_variable() == false);
//    CHECK((new NumExpr(-11))->has_variable() == false);
//}

TEST_CASE("NumExpr subst() tests") {
    CHECK((NEW(NumExpr)(1))->subst("x", NEW(VarExpr)("y"))->equals(NEW(NumExpr)(1)));
}

TEST_CASE("NumExpr print()/to_string() tests") {
    CHECK((NEW(NumExpr)(1))->to_string() == "1");
}

TEST_CASE("NumExpr pretty_print() tests") {
    std::stringstream out("");
    (NEW(NumExpr)(4))->pretty_print(out);
    CHECK(out.str() == "4");
}