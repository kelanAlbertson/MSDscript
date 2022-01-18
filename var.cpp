//
// Created by Kelan Albertson on 1/17/22.
//

#include <string>

#include "catch.h"
#include "var.h"
#include "num.h"

Var::Var(std::string name, int value) {
    this->name_ = name;
    this->value_ = value;
}

bool Var::equals(Expr *other) {
    Var* v = dynamic_cast<Var*>(other);
    if (v == nullptr) {
        return false;
    }
    else {
        return (this->name_ == v->name_ && this->value_ == v->value_);
    }
}

TEST_CASE("Var Tests") {
    CHECK((new Var("one", 1))->equals(new Var("one", 1)) == true);
    CHECK((new Var("one", 1))->equals(new Var("ONE", 1)) == false);
    CHECK((new Var("one", 1))->equals(new Var("two", 2)) == false);
    CHECK((new Var("one", 1))->equals(new Num(1)) == false);
}