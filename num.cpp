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
    if (n == NULL) {
        return false;
    }
    else {
        return (this->value_ == n->value_);
    }
}

TEST_CASE("Num") {
    CHECK((new Num(8))->equals(new Num(8)) == true);
}
