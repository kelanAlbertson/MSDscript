//
// Created by Kelan Albertson on 2/26/22.
//

#include "FunVal.h"
#include "VarExpr.h"
#include "catch.h"
#include "AddExpr.h"
#include "NumExpr.h"
#include "FunExpr.h"
#include "BoolVal.h"
#include "NumVal.h"
#include "BoolExpr.h"
#include "ExtendedEnv.h"
#include "Step.h"

FunVal::FunVal(PTR(VarExpr) arg, PTR(Expr) body, PTR(Env) env) {
    this->arg_ = arg;
    this->body_ = body;
    this->env_ = env;
}

bool FunVal::equals(PTR(Val)other) {
    PTR(FunVal)fv = CAST(FunVal)(other);
    if (fv == nullptr) {
        return false;
    }
    else {
        return (this->arg_->equals(fv->arg_) && this->body_->equals(fv->body_) && this->env_->equals(fv->env_));
    }
}

std::string FunVal::to_string() {
//    return "(_fun (" + this->arg_->to_string() + ") " + this->body_->to_string() + ")";
    return "[function]";
}

//PTR(Expr)FunVal::to_expr() {
//    return NEW(FunExpr)(this->arg_, this->body_);
//}

PTR(Val)FunVal::add_to(PTR(Val) other) {
    throw std::runtime_error("Cannot add_to() with a FunVal");
}

PTR(Val)FunVal::multiply_by(PTR(Val) other) {
    throw std::runtime_error("Cannot multiply_by() with a FunVal");
}

bool FunVal::is_true() {
    throw std::runtime_error("Cannot use is_true() on a FunVal");
}

PTR(Val) FunVal::call(PTR(Val) arg) {
    return this->body_->interp(NEW(ExtendedEnv)(this->arg_->name_, arg, this->env_));
}

void FunVal::call_step(PTR(Val) arg_val, PTR(Cont) rest) {
    Step::mode_ = Step::interp_mode;
    Step::expr_ = body_;
    Step::env_ = NEW(ExtendedEnv)(arg_->name_, arg_val, env_);
    Step::cont_ = rest;
}

/**
 *************************   TESTS   **************************
 **/

TEST_CASE("FunVal equals() tests") {
    CHECK((NEW(FunVal)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)), Env::empty))
                  ->equals(NEW(FunVal)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)), Env::empty)) == true);
    CHECK((NEW(FunVal)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)), NEW(ExtendedEnv)("x", NEW(NumVal)(0), Env::empty)))
                  ->equals(NEW(FunVal)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)), Env::empty)) == false);
    CHECK((NEW(FunVal)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)), Env::empty))
                  ->equals(NEW(FunVal)(NEW(VarExpr)("x"), NEW(NumExpr)(1), Env::empty)) == false);
    CHECK((NEW(FunVal)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)), Env::empty))
                  ->equals(NEW(BoolVal)(true)) == false);
}

TEST_CASE("FunVal to_string() tests") {
    CHECK((NEW(FunVal)(NEW(VarExpr)("x"), NEW(NumExpr)(1), Env::empty))->to_string() == "[function]"); // was "(_fun (x) 1)"
    CHECK((NEW(FunVal)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)), Env::empty))
                  ->to_string() == "[function]"); // was "(_fun (x) (x+1))"
    CHECK((NEW(FunVal)(NEW(VarExpr)("x"), NEW(VarExpr)("y"), Env::empty))->to_string() == "[function]"); // was "(_fun (x) y)"
}

//TEST_CASE("FunVal to_expr() tests") {
//    CHECK((NEW(FunVal)(NEW(VarExpr)("y"), NEW(VarExpr)("y")))->to_expr()
//            ->equals(NEW(FunExpr)(NEW(VarExpr)("y"), NEW(VarExpr)("y"))));
//    CHECK((NEW(FunVal)(NEW(VarExpr)("y"), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(1))))->to_expr()
//                  ->equals(NEW(FunExpr)(NEW(VarExpr)("y"), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(1)))));
//}

TEST_CASE("FunVal add_to() tests") {
    CHECK_THROWS_WITH((NEW(FunVal)(NEW(VarExpr)("y"), NEW(VarExpr)("y"), Env::empty))->add_to(NEW(NumVal)(1)), "Cannot add_to() with a FunVal");
}

TEST_CASE("FunVal multiply_by() tests") {
    CHECK_THROWS_WITH((NEW(FunVal)(NEW(VarExpr)("y"), NEW(VarExpr)("y"), Env::empty))->multiply_by(NEW(NumVal)(1)), "Cannot multiply_by() with a FunVal");
}

TEST_CASE("FunVal is_true() tests") {
    CHECK_THROWS_WITH((NEW(FunVal)(NEW(VarExpr)("x"), NEW(NumExpr)(1), Env::empty))->is_true(), "Cannot use is_true() on a FunVal");
}

TEST_CASE("FunVal call() tests") {
    CHECK((NEW(FunVal)(NEW(VarExpr)("x"), NEW(NumExpr)(1), Env::empty))->call(NEW(NumVal)(5))->equals(NEW(NumVal)(1)));
    CHECK((NEW(FunVal)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)), Env::empty))->call(NEW(NumVal)(5))->equals(NEW(NumVal)(6)));
    CHECK((NEW(FunVal)(NEW(VarExpr)("x"), NEW(BoolExpr)(true), Env::empty))->call(NEW(NumVal)(5))->equals(NEW(BoolVal)(true)));
    CHECK_THROWS_WITH((NEW(FunVal)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(BoolExpr)(true)), Env::empty))
            ->call(NEW(NumVal)(5))->equals(NEW(BoolVal)(true)), "Cannot use add_to() with a non-number");
}
