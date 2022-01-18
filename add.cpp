//
// Created by Kelan Albertson on 1/16/22.
//

#include "add.h"
#include "catch.h"
#include "num.h"

Add::Add(Expr* lhs, Expr* rhs) {
    this->lhs_ = lhs;
    this->rhs_ = rhs;
}

bool Add::equals(Expr* other) {
    Add* a = dynamic_cast<Add*>(other);
    if (a == nullptr) {
        return false;
    }
    else {
        return (this->lhs_->equals(a->lhs_) && this->rhs_->equals(a->rhs_));
    }
}

TEST_CASE("Add Equals Tests") {
    CHECK((new Add(new Num(0), new Num(1)))->equals(new Add(new Num(0), new Num(1))) == true);
    CHECK((new Add(new Num(0), new Num(1)))->equals(new Add(new Num(1), new Num(0))) == false);
    CHECK((new Add(new Num(0), new Num(1)))->equals(new Add(new Num(-20), new Num(9))) == false);
    CHECK((new Add(new Num(0), new Num(1)))->equals(new Num(1)) == false);
}
