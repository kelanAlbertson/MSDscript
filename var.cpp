//
// Created by Kelan Albertson on 1/17/22.
//

#include <string>

#include "catch.h"
#include "var.h"
#include "num.h"
#include "stdexcept"

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