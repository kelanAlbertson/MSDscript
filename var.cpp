//
// Created by Kelan Albertson on 1/17/22.
//

#include <string>

#include "catch.h"
#include "var.h"
#include "num.h"

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

TEST_CASE("Var Equals Tests") {
    CHECK((new Var("one"))->equals(new Var("one")) == true);
    CHECK((new Var("one"))->equals(new Var("ONE")) == false);
    CHECK((new Var("one"))->equals(new Var("two")) == false);
    CHECK((new Var("one"))->equals(new Num(1)) == false);
}