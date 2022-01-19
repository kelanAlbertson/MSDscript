//
// Created by Kelan Albertson on 1/16/22.
//

#include "num.h"
#include "catch.h"

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

TEST_CASE("Num equals() tests") {
    CHECK((new Num(0))->equals(new Num(0)) == true);
    CHECK((new Num(8))->equals(new Num(8)) == true);
    CHECK((new Num(8))->equals(new Num(-1)) == false);
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
