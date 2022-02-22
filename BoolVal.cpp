//
// Created by Kelan Albertson on 2/20/22.
//

#include "BoolVal.h"
#include "BoolExpr.h"
#include "NumVal.h"
#include "catch.h"

BoolVal::BoolVal(bool rep) {
    this->rep_ = rep;
}

bool BoolVal::equals(Val *other) {
    BoolVal *bv = dynamic_cast<BoolVal*>(other);
    if (bv == nullptr) {
        return false;
    }
    else {
        return (this->rep_ == bv->rep_);
    }
}

std::string BoolVal::to_string() {
    if (this->rep_) {
        return "_true";
    }
    else {
        return "_false";
    }
}

Expr *BoolVal::to_expr() {
    return nullptr; //TODO
//    return new BoolExpr(this->rep_);
}

Val *BoolVal::add_to(Val *other) {
    throw std::runtime_error("Cannot add_to() with a BoolVal");
}

Val *BoolVal::multiply_by(Val *other) {
    throw std::runtime_error("Cannot multiply_by() with a BoolVal");
}

/**
 *************************   TESTS   **************************
 **/

TEST_CASE("BoolVal equals() tests") {
    CHECK((new BoolVal(true))->equals(new BoolVal(true)) == true);
    CHECK((new BoolVal(false))->equals(new BoolVal(true)) == false);
    CHECK((new BoolVal(true))->equals(new NumVal(1)) == false);
}

TEST_CASE("BoolVal to_string() tests") {
    CHECK((new BoolVal(true))->to_string() == "_true");
    CHECK((new BoolVal(false))->to_string() == "_false");
}

TEST_CASE("BoolVal to_expr() tests") {
    //TODO
}

TEST_CASE("BoolVal add_to() tests") {
    CHECK_THROWS_WITH((new BoolVal(true))->add_to(new BoolVal(true)), "Cannot add_to() with a BoolVal");
    CHECK_THROWS_WITH((new BoolVal(false))->add_to(new NumVal(0)), "Cannot add_to() with a BoolVal");
}

TEST_CASE("BoolVal multiply_by() tests") {
    CHECK_THROWS_WITH((new BoolVal(true))->multiply_by(new BoolVal(true)), "Cannot multiply_by() with a BoolVal");
    CHECK_THROWS_WITH((new BoolVal(false))->multiply_by(new NumVal(0)), "Cannot multiply_by() with a BoolVal");
}