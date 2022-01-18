//
// Created by Kelan Albertson on 1/16/22.
//

#include "num.h"
#include "catch.h"

Num :: Num(int value) {
    this->value_ = value;
}

bool Num :: equals(Expr* other) {
    Num* n = dynamic_cast<Num*>(other);
    if (n == nullptr) {
        return false;
    }
    else {
        return (this->value_ == n->value_);
    }
}

TEST_CASE("Num Tests") {
    CHECK((new Num(0))->equals(new Num(0)) == true);
    CHECK((new Num(8))->equals(new Num(8)) == true);
    CHECK((new Num(8))->equals(new Num(-1)) == false);
}
