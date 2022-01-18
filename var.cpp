//
// Created by Kelan Albertson on 1/17/22.
//

#include <string>

#include "catch.h"
#include "var.h"

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

}