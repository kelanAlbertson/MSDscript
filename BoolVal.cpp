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

bool BoolVal::equals(PTR(Val)other) {
    PTR(BoolVal) bv = CAST(BoolVal)(other);
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

//PTR(Expr) BoolVal::to_expr() {
//    return NEW(BoolExpr)(this->rep_);
//}

PTR(Val) BoolVal::add_to(PTR(Val)other) {
    throw std::runtime_error("Cannot add_to() with a BoolVal");
}

PTR(Val) BoolVal::multiply_by(PTR(Val)other) {
    throw std::runtime_error("Cannot multiply_by() with a BoolVal");
}

bool BoolVal::is_true() {
    return this->rep_;
}

PTR(Val) BoolVal::call(PTR(Val) arg) {
    throw std::runtime_error("Cannot call() a BoolVal");
}

void call_step(PTR(Val) arg_val, PTR(Cont) rest) {
    throw std::runtime_error("Cannot step_call() a BoolVal");
}

/**
 *************************   TESTS   **************************
 **/

TEST_CASE("BoolVal equals() tests") {
    CHECK((NEW(BoolVal)(true))->equals(NEW(BoolVal)(true)) == true);
    CHECK((NEW(BoolVal)(false))->equals(NEW(BoolVal)(true)) == false);
    CHECK((NEW(BoolVal)(true))->equals(NEW(NumVal)(1)) == false);
}

TEST_CASE("BoolVal to_string() tests") {
    CHECK((NEW(BoolVal)(true))->to_string() == "_true");
    CHECK((NEW(BoolVal)(false))->to_string() == "_false");
}

//TEST_CASE("BoolVal to_expr() tests") {
//    CHECK((NEW(BoolVal)(true))->to_expr()->equals(NEW(BoolExpr)(true)));
//    CHECK((NEW(BoolVal)(false))->to_expr()->equals(NEW(BoolExpr)(false)));
//}

TEST_CASE("BoolVal add_to() tests") {
    CHECK_THROWS_WITH((NEW(BoolVal)(true))->add_to(NEW(BoolVal)(true)), "Cannot add_to() with a BoolVal");
    CHECK_THROWS_WITH((NEW(BoolVal)(false))->add_to(NEW(NumVal)(0)), "Cannot add_to() with a BoolVal");
}

TEST_CASE("BoolVal multiply_by() tests") {
    CHECK_THROWS_WITH((NEW(BoolVal)(true))->multiply_by(NEW(BoolVal)(true)), "Cannot multiply_by() with a BoolVal");
    CHECK_THROWS_WITH((NEW(BoolVal)(false))->multiply_by(NEW(NumVal)(0)), "Cannot multiply_by() with a BoolVal");
}

TEST_CASE("BoolVal is_true() tests") {
    CHECK((NEW(BoolVal)(true))->is_true() == true);
    CHECK((NEW(BoolVal)(false))->is_true() == false);
}

TEST_CASE("BoolVal call() tests") {
    CHECK_THROWS_WITH((NEW(BoolVal)(99))->call(NEW(BoolVal)(2)), "Cannot call() a BoolVal");
}