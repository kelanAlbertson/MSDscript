//
// Created by Kelan Albertson on 1/17/22.
//

#include "VarExpr.h"
#include "AddExpr.h"
#include "NumExpr.h"
#include "MultExpr.h"
#include "Env.h"
#include "catch.h"
#include "stdexcept"
#include <sstream>

VarExpr::VarExpr(std::string name) {
    this->name_ = name;
}

bool VarExpr::equals(PTR(Expr)other) {
    PTR(VarExpr) v = CAST(VarExpr)(other);
    if (v == nullptr) {
        return false;
    }
    else {
        return (this->name_ == v->name_);
    }
}

PTR(Val) VarExpr::interp(PTR(Env) env) {
    throw std::runtime_error("VarExpr cannot be interpreted");
}

//bool VarExpr::has_variable() {
//    return true;
//}

//PTR(Expr) VarExpr::subst(std::string variableName, PTR(Expr) replacement) {
//    if(this->name_ == variableName) {
//        return replacement;
//    }
//    else {
//        return THIS;
//    }
//}

void VarExpr::print(std::ostream &out) {
    out << this->name_;
}

void VarExpr::pretty_print_at(std::ostream &out, precedence_t prec, bool keyword_parentheses, std::streampos &pos) {
    out << this->name_;
}

/**
 *************************   TESTS   **************************
 **/

TEST_CASE("VarExpr equals() tests") {
    CHECK((NEW(VarExpr)("one"))->equals(NEW(VarExpr)("one")) == true);
    CHECK((NEW(VarExpr)("one"))->equals(NEW(VarExpr)("ONE")) == false);
    CHECK((NEW(VarExpr)("one"))->equals(NEW(VarExpr)("two")) == false);
    CHECK((NEW(VarExpr)("one"))->equals(NEW(NumExpr)(1)) == false);
}
TEST_CASE("VarExpr interp() tests") {
    CHECK_THROWS_WITH((NEW(VarExpr)("x"))->interp(Env::empty), "VarExpr cannot be interpreted");
}

//TEST_CASE("VarExpr has_variable() tests") {
//    CHECK((new VarExpr(""))->has_variable() == true);
//    CHECK((new VarExpr("test"))->has_variable() == true);
//}

//TEST_CASE("VarExpr subst() tests") {
//    CHECK((NEW(VarExpr)("x"))->subst("x", NEW(VarExpr)("y"))->equals(NEW(VarExpr)("y")));
//}

TEST_CASE("VarExpr print()/to_string() tests") {
    CHECK((NEW(VarExpr)("x"))->to_string() == "x");
}

TEST_CASE("VarExpr pretty_print() tests") {
    std::stringstream out("");
    (NEW(VarExpr)("x"))->pretty_print(out);
    CHECK(out.str() == "x");
    out.str(std::string());
    (NEW(MultExpr)(NEW(AddExpr)(NEW(NumExpr)(1), NEW(VarExpr)("x")), NEW(NumExpr)(3)))->pretty_print(out);
    CHECK(out.str() == "(1 + x) * 3");
    out.str(std::string());
    (NEW(AddExpr)(NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(3), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(-2))), NEW(MultExpr)(NEW(NumExpr)(-2), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(-3)))), NEW(NumExpr)(-5)))->pretty_print(out);
    CHECK(out.str() == "(3 * (x + -2) + -2 * (x + -3)) + -5");
}