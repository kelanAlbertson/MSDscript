//
// Created by Kelan Albertson on 1/17/22.
//

#include "var.h"
#include "add.h"
#include "num.h"
#include "mult.h"
#include "catch.h"
#include "stdexcept"
#include <sstream>

Var::Var(std::string name) {
    this->name_ = name;
}

bool Var::equals(Expr *other) {
    Var* v = dynamic_cast<Var*>(other);
    if (v == nullptr) {
        return false;
    }
    else {
        return (this->name_ == v->name_);
    }
}

int Var::interp() {
    throw std::runtime_error("Error: Var cannot be interpreted");
}

bool Var::has_variable() {
    return true;
}

Expr* Var::subst(std::string variableName, Expr* replacement) {
    if(this->name_ == variableName) {
        return replacement;
    }
    else {
        return new Var(this->name_);
    }
}

void Var::print(std::ostream &out) {
    out << this->name_;
}

void Var::pretty_print_at(std::ostream &out, Expr::precedence_t prec) {
    out << this->name_;
}

/**
 *************************   TESTS   **************************
 **/

TEST_CASE("Var equals() tests") {
    CHECK((new Var("one"))->equals(new Var("one")) == true);
    CHECK((new Var("one"))->equals(new Var("ONE")) == false);
    CHECK((new Var("one"))->equals(new Var("two")) == false);
    CHECK((new Var("one"))->equals(new Num(1)) == false);
}
TEST_CASE("Var interp() tests") {
    CHECK_THROWS_WITH( (new Var("x"))->interp(), "Error: Var cannot be interpreted");
}

TEST_CASE("Var has_variable() tests") {
    CHECK((new Var(""))->has_variable() == true);
    CHECK((new Var("test"))->has_variable() == true);
}

TEST_CASE("Var subst() tests") {
    CHECK((new Var("x"))->subst("x", new Var("y"))->equals(new Var("y")));
}

TEST_CASE("Var print()/to_string() tests") {
    CHECK((new Var("x"))->to_string() == "x");
}

TEST_CASE("Var pretty_print() tests") {
    std::stringstream out("");
    (new Var("x"))->pretty_print(out);
    CHECK(out.str() == "x");
    out.str(std::string());
    (new Mult(new Add(new Num(1), new Var("x")), new Num(3)))->pretty_print(out);
    CHECK(out.str() == "(1 + x) * 3");
    out.str(std::string());
    (new Add(new Add(new Mult(new Num(3), new Add(new Var("x"), new Num(-2))), new Mult(new Num(-2), new Add(new Var("x"), new Num(-3)))), new Num(-5)))->pretty_print(out);
    CHECK(out.str() == "(3 * (x + -2) + -2 * (x + -3)) + -5");
}