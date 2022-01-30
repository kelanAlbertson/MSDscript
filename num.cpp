//
// Created by Kelan Albertson on 1/16/22.
//

#include "num.h"
#include "var.h"
#include "catch.h"
#include <sstream>

Num::Num(int value) {
    this->value_ = value;
}

bool Num::equals(Expr* other) {
    Num* n = dynamic_cast<Num*>(other);
    if (n == nullptr) {
        return false;
    }
    else {
        return (this->value_ == n->value_);
    }
}

int Num::interp() {
    return this->value_;
}

bool Num::has_variable() {
    return false;
}

Expr* Num::subst(std::string variableName, Expr *replacement) {
    return this;
}

void Num::print(std::ostream &out) {
    out << this->value_;
}

void Num::pretty_print_at(std::ostream &out, Expr::precedence_t prec) {
    out << this->value_;
}

/**
 *************************   TESTS   **************************
 **/

TEST_CASE("Num equals() tests") {
    CHECK((new Num(0))->equals(new Num(0)) == true);
    CHECK((new Num(8))->equals(new Num(8)) == true);
    CHECK((new Num(8))->equals(new Num(-1)) == false);
    CHECK((new Num(8))->equals(new Var("8")) == false);
}

TEST_CASE("Num interp() tests") {
    CHECK((new Num(0))->interp() == 0);
    CHECK((new Num(1))->interp() == 1);
    CHECK((new Num(99))->interp() == 99);
    CHECK((new Num(-1))->interp() == -1);
}

TEST_CASE("Num has_variable() tests") {
    CHECK((new Num(0))->has_variable() == false);
    CHECK((new Num(-11))->has_variable() == false);
}

TEST_CASE("Num subst() tests") {
    CHECK((new Num(1))->subst("x", new Var("y"))->equals(new Num(1)));
}

TEST_CASE("Num print()/to_string() tests") {
    CHECK((new Num(1))->to_string() == "1");
}

TEST_CASE("Num pretty_print() tests") {
    std::stringstream out("");
    (new Num(4))->pretty_print(out);
    CHECK(out.str() == "4");
}