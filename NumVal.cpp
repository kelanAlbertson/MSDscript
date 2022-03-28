//
// Created by Kelan Albertson on 2/16/22.
//

#include <stdexcept>
#include "NumVal.h"
#include "NumExpr.h"
#include "BoolVal.h"
#include "catch.h"

NumVal::NumVal(int rep) {
    this->rep_ = rep;
}

bool NumVal::equals(PTR(Val) other) {
    PTR(NumVal) nv = CAST(NumVal)(other);
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

//PTR(Expr)NumVal::to_expr() {
//    return NEW(NumExpr)(this->rep_);
//}

PTR(Val)NumVal::add_to(PTR(Val)other) {
    PTR(NumVal) nv = CAST(NumVal)(other);
    if (nv == nullptr) {
        throw std::runtime_error("Cannot use add_to() with a non-number");
    }
    return NEW(NumVal)(rep_ + nv->rep_);
}

PTR(Val)NumVal::multiply_by(PTR(Val)other) {
    PTR(NumVal) nv = CAST(NumVal)(other);
    if (nv == nullptr) {
        throw std::runtime_error("Cannot use multiply_by() with a non-number");
    }
    return NEW(NumVal)(rep_* nv->rep_);
}

bool NumVal::is_true() {
    throw std::runtime_error("Cannot use is_true() on a NumVal");
}

PTR(Val) NumVal::call(PTR(Val) arg) {
    throw std::runtime_error("Cannot call() a NumVal");
}

void NumVal::call_step(PTR(Val) arg_val, PTR(Cont) rest) {
    throw std::runtime_error("Cannot call_step() a NumVal");
}

/**
 *************************   TESTS   **************************
 **/

TEST_CASE("NumVal equals() tests") {
    CHECK((NEW(NumVal)(1))->equals(NEW(NumVal)(1)) == true);
    CHECK((NEW(NumVal)(-99))->equals(NEW(NumVal)(99999)) == false);
    CHECK((NEW(NumVal)(0))->equals(NEW(BoolVal)(false)) == false); // NOTE: using nullptr is bad practice, should change this in the future
}

TEST_CASE("NumVal to_string() tests") {
    CHECK((NEW(NumVal)(0))->to_string() == "0");
    CHECK((NEW(NumVal)(123))->to_string() == "123");
}

//TEST_CASE("NumVal to_expr() tests") {
//    CHECK((NEW(NumVal)(1))->to_expr()->equals(NEW(NumExpr)(1)));
//    CHECK((NEW(NumVal)(-99))->to_expr()->equals(NEW(NumExpr)(-99)));
//}

TEST_CASE("NumVal add_to() tests") {
    CHECK((NEW(NumVal)(1))->add_to(NEW(NumVal)(5))->equals(NEW(NumVal)(6)));
    CHECK_THROWS_WITH((NEW(NumVal)(8))->add_to(nullptr), "Cannot use add_to() with a non-number");
}

TEST_CASE("NumVal multiply_by() tests") {
    CHECK((NEW(NumVal)(-4))->multiply_by(NEW(NumVal)(5))->equals(NEW(NumVal)(-20)));
    CHECK_THROWS_WITH((NEW(NumVal)(-1))->multiply_by(nullptr), "Cannot use multiply_by() with a non-number");
}

TEST_CASE("NumVal is_true() tests") {
    CHECK_THROWS_WITH((NEW(NumVal)(1))->is_true(), "Cannot use is_true() on a NumVal");
}

TEST_CASE("NumVal call() tests") {
    CHECK_THROWS_WITH((NEW(NumVal)(99))->call(NEW(NumVal)(2)), "Cannot call() a NumVal");
}