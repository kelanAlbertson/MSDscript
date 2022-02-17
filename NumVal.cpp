//
// Created by Kelan Albertson on 2/16/22.
//

#include <stdexcept>
#include "NumVal.h"
#include "NumExpr.h"
#include "catch.h"

NumVal::NumVal(int rep) {
    this->rep_ = rep;
}

bool NumVal::equals(Val* other) {
    NumVal *nv = dynamic_cast<NumVal*>(other);
    if (nv == nullptr) {
        return false;
    }
    else {
        return (this->rep_ == nv->rep_);
    }
}

std::string NumVal::to_string() {
    return std::to_string(this->rep_);
}

Expr *NumVal::to_expr() {
    return new NumExpr(this->rep_);
}

Val *NumVal::add_to(Val *other) {
    NumVal *nv = dynamic_cast<NumVal*>(other);
    if (nv == nullptr) {
        throw std::runtime_error("cannot add with a non-number");
    }
    return new NumVal(rep_ + nv->rep_);
}

Val *NumVal::multiply_by(Val *other) {
    NumVal *nv = dynamic_cast<NumVal*>(other);
    if (nv == nullptr) {
        throw std::runtime_error("cannot multiply with a non-number");
    }
    return new NumVal(rep_ * nv->rep_);
}

/**
 *************************   TESTS   **************************
 **/

TEST_CASE("NumVal equals() tests") {
    CHECK((new NumVal(1))->equals(new NumVal(1)) == true);
    CHECK((new NumVal(-99))->equals(new NumVal(99999)) == false);
    CHECK((new NumVal(0))->equals(nullptr) == false); // NOTE: using nullptr is bad practice, should change this in the future
}

TEST_CASE("NumVal to_string() tests") {
    CHECK((new NumVal(0))->to_string() == "0");
    CHECK((new NumVal(123))->to_string() == "123");
}

TEST_CASE("NumVal to_expr() tests") {
    CHECK((new NumVal(1))->to_expr()->equals(new NumExpr(1)));
    CHECK((new NumVal(-99))->to_expr()->equals(new NumExpr(-99)));
}

TEST_CASE("NumVal add_to() tests") {
    CHECK((new NumVal(1))->add_to(new NumVal(5))->equals(new NumVal(6)));
    CHECK_THROWS_WITH((new NumVal(8))->add_to(nullptr), "cannot add with a non-number");
}

TEST_CASE("NumVal multiply_by() tests") {
    CHECK((new NumVal(-4))->multiply_by(new NumVal(5))->equals(new NumVal(-20)));
    CHECK_THROWS_WITH((new NumVal(-1))->multiply_by(nullptr), "cannot multiply with a non-number");
}