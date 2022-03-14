//
// Created by Kelan Albertson on 1/17/22.
//

#include "VarExpr.h"
#include "AddExpr.h"
#include "NumExpr.h"
#include "MultExpr.h"
#include "catch.h"
#include "stdexcept"
#include <sstream>

VarExpr::VarExpr(std::string name) {
    this->name_ = name;
}

bool VarExpr::equals(Expr *other) {
    VarExpr* v = dynamic_cast<VarExpr*>(other);
    if (v == nullptr) {
        return false;
    }
    else {
        return (this->name_ == v->name_);
    }
}

Val * VarExpr::interp() {
    throw std::runtime_error("VarExpr cannot be interpreted");
}

//bool VarExpr::has_variable() {
//    return true;
//}

Expr* VarExpr::subst(std::string variableName, Expr* replacement) {
    if(this->name_ == variableName) {
        return replacement;
    }
    else {
        return this;
    }
}

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
    CHECK((new VarExpr("one"))->equals(new VarExpr("one")) == true);
    CHECK((new VarExpr("one"))->equals(new VarExpr("ONE")) == false);
    CHECK((new VarExpr("one"))->equals(new VarExpr("two")) == false);
    CHECK((new VarExpr("one"))->equals(new NumExpr(1)) == false);
}
TEST_CASE("VarExpr interp() tests") {
    CHECK_THROWS_WITH((new VarExpr("x"))->interp(), "VarExpr cannot be interpreted");
}

//TEST_CASE("VarExpr has_variable() tests") {
//    CHECK((new VarExpr(""))->has_variable() == true);
//    CHECK((new VarExpr("test"))->has_variable() == true);
//}

TEST_CASE("VarExpr subst() tests") {
    CHECK((new VarExpr("x"))->subst("x", new VarExpr("y"))->equals(new VarExpr("y")));
}

TEST_CASE("VarExpr print()/to_string() tests") {
    CHECK((new VarExpr("x"))->to_string() == "x");
}

TEST_CASE("VarExpr pretty_print() tests") {
    std::stringstream out("");
    (new VarExpr("x"))->pretty_print(out);
    CHECK(out.str() == "x");
    out.str(std::string());
    (new MultExpr(new AddExpr(new NumExpr(1), new VarExpr("x")), new NumExpr(3)))->pretty_print(out);
    CHECK(out.str() == "(1 + x) * 3");
    out.str(std::string());
    (new AddExpr(new AddExpr(new MultExpr(new NumExpr(3), new AddExpr(new VarExpr("x"), new NumExpr(-2))), new MultExpr(new NumExpr(-2), new AddExpr(new VarExpr("x"), new NumExpr(-3)))), new NumExpr(-5)))->pretty_print(out);
    CHECK(out.str() == "(3 * (x + -2) + -2 * (x + -3)) + -5");
}