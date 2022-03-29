//
// Created by Kelan Albertson on 3/28/22.
//

#include "Step.h"
#include "Env.h"
#include "Cont.h"
#include "Expr.h"
#include "AddExpr.h"
#include "NumExpr.h"
#include "Val.h"
#include "NumVal.h"
#include "parse.h"
#include "catch.h"

Step::mode_t Step::mode_;
PTR(Expr) Step::expr_;
PTR(Env) Step::env_;
PTR(Val) Step::val_;
PTR(Cont) Step::cont_;

PTR(Val) Step::interp_by_steps(PTR(Expr) e) {
    Step::mode_ = Step::interp_mode;
    Step::expr_ = e;
    Step::env_ = Env::empty;
    Step::val_ = nullptr;
    Step::cont_ = Cont::done;

    while(1) {
        if (Step::mode_ == Step::interp_mode) {
            Step::expr_->step_interp();
        }
        else {
            if (Step::cont_ == Cont::done) {
                return Step::val_;
            }
            else {
                Step::cont_->step_continue();
            }
        }
    }
}

/**
 *************************   TESTS   **************************
 **/

TEST_CASE("Matthew's countdown test") {
    CHECK((Step::interp_by_steps(parse_string(
            "_let countdown = _fun(countdown)"
            "                  _fun(n)"
            "                   _if n == 0"
            "                   _then 0"
            "                   _else countdown(countdown)(n + -1)"
            "_in countdown(countdown)(1000000)")))
            ->equals(NEW(NumVal)(0)));
}

TEST_CASE("AddExpr interp_by_steps() tests") {
    CHECK(Step::interp_by_steps(NEW(AddExpr)(NEW(NumExpr)(0), NEW(NumExpr)(0)))->equals(NEW(NumVal)(0)));
    CHECK(Step::interp_by_steps(NEW(AddExpr)(NEW(NumExpr)(0), NEW(NumExpr)(1)))->equals(NEW(NumVal)(1)));
    CHECK(Step::interp_by_steps(NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(0)))->equals(NEW(NumVal)(1)));
    CHECK(Step::interp_by_steps(NEW(AddExpr)(NEW(NumExpr)(-5), NEW(NumExpr)(18)))->equals(NEW(NumVal)(13)));
}

TEST_CASE("BoolExpr interp_by_steps() tests") {
    
}
