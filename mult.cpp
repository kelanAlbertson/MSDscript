//
// Created by Kelan Albertson on 1/16/22.
//

#include "add.h"
#include "catch.h"
#include "mult.h"
#include "num.h"

Mult::Mult(Expr *lhs, Expr *rhs) {
    this->lhs_ = lhs;
    this->rhs_ = rhs;
}

bool Mult::equals(Expr *other) {
    Mult* m = dynamic_cast<Mult*>(other);
    if (m == nullptr) {
        return false;
    }
    else {
        return (this->lhs_->equals(m->lhs_) && this->rhs_->equals(m->rhs_));
    }
}

int Mult::interp() {
    return (lhs_->interp() * rhs_->interp());
}

TEST_CASE("Mult equals() tests") {
    CHECK((new Mult(new Num(-1), new Num(0)))->equals(new Mult(new Num(-1), new Num(0))) == true);
    CHECK((new Mult(new Num(-1), new Num(0)))->equals(new Mult(new Num(0), new Num(-1))) == false);
    CHECK((new Mult(new Num(-1), new Num(0)))->equals(new Mult(new Num(100), new Num(0))) == false);
    CHECK((new Mult(new Num(-1), new Num(0)))->equals(new Add(new Num(-1), new Num(0))) == false);
}
TEST_CASE("Mult interp() tests") {
    CHECK((new Mult(new Num(0), new Num(0)))->interp() == 0);
    CHECK((new Mult(new Num(0), new Num(1)))->interp() == 0);
    CHECK((new Mult(new Num(-1), new Num(10)))->interp() == -10);
    CHECK((new Mult(new Num(-5), new Num(-5)))->interp() == 25);
}