//
// Created by Kelan Albertson on 1/16/22.
//

#include "add.h"
#include "catch.h"
#include "mult.h"
#include "num.h"
#include "var.h"

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
    return (this->lhs_->interp() * this->rhs_->interp());
}

bool Mult::has_variable() {
    return (this->lhs_->has_variable() || this->rhs_->has_variable());
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

TEST_CASE("Mult has_variable() tests") {
    CHECK((new Mult(new Num(0), new Num(1)))->has_variable() == false);
    CHECK((new Mult(new Var("test"), new Num(1)))->has_variable() == true);
    CHECK((new Mult(new Var("test"), new Var("test")))->has_variable() == true);
}