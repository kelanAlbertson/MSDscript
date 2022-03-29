//
// Created by Kelan Albertson on 3/28/22.
//

#include "Step.h"
#include "Env.h"
#include "Cont.h"
#include "Expr.h"
#include "AddExpr.h"
#include "BoolExpr.h"
#include "CallExpr.h"
#include "EqExpr.h"
#include "NumExpr.h"
#include "VarExpr.h"
#include "Val.h"
#include "BoolVal.h"
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

//TEST_CASE("Matthew's countdown test") {
//    CHECK((Step::interp_by_steps(parse_string(
//            "_let countdown = _fun(countdown)"
//            "                  _fun(n)"
//            "                   _if n == 0"
//            "                   _then 0"
//            "                   _else countdown(countdown)(n + -1)"
//            "_in countdown(countdown)(1000000)")))
//            ->equals(NEW(NumVal)(0)));
//}

TEST_CASE("AddExpr interp_by_steps() tests") {
    CHECK(Step::interp_by_steps(NEW(AddExpr)(NEW(NumExpr)(0), NEW(NumExpr)(0)))->equals(NEW(NumVal)(0)));
    CHECK(Step::interp_by_steps(NEW(AddExpr)(NEW(NumExpr)(0), NEW(NumExpr)(1)))->equals(NEW(NumVal)(1)));
    CHECK(Step::interp_by_steps(NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(0)))->equals(NEW(NumVal)(1)));
    CHECK(Step::interp_by_steps(NEW(AddExpr)(NEW(NumExpr)(-5), NEW(NumExpr)(18)))->equals(NEW(NumVal)(13)));
}

TEST_CASE("BoolExpr interp_by_steps() tests") {
    CHECK(Step::interp_by_steps(NEW(BoolExpr)(true))->equals(NEW(BoolVal)(true)));
    CHECK(Step::interp_by_steps(NEW(BoolExpr)(false))->equals(NEW(BoolVal)(false)));
}

TEST_CASE("CallExpr interp_by_steps() tests") {
    CHECK(Step::interp_by_steps(parse_string("(_fun (x) x+1)(5)"))->equals(NEW(NumVal)(6)));
    CHECK(Step::interp_by_steps(parse_string("(_fun (x) x*x)(3)"))->equals(NEW(NumVal)(9)));
    CHECK(Step::interp_by_steps(parse_string("(_fun (x) (_fun (y) x*x + y*y) (3))(2)"))->equals(NEW(NumVal)(13)));
    CHECK(Step::interp_by_steps(parse_string("(_fun (b) _true)(-99)"))->equals(NEW(BoolVal)(true)));
    CHECK_THROWS_WITH(Step::interp_by_steps(NEW(CallExpr)(NEW(BoolExpr)(false), NEW(NumExpr)(1))), "Cannot step_call() a BoolVal");
}

TEST_CASE("EqExpr interp_by_steps() tests") {
    CHECK(Step::interp_by_steps(parse_string("0==0"))->equals(NEW(BoolVal)(true)));
    CHECK(Step::interp_by_steps(parse_string("-99==0"))->equals(NEW(BoolVal)(false)));
    CHECK(Step::interp_by_steps(parse_string("_true==_true"))->equals(NEW(BoolVal)(true)));
    CHECK(Step::interp_by_steps(parse_string("_true==1"))->equals(NEW(BoolVal)(false)));
    CHECK_THROWS_WITH(Step::interp_by_steps(NEW(EqExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2)))->equals(NEW(BoolVal)(false)), "free variable: x");
}

TEST_CASE("IfExpr interp_by_steps() tests") {
    CHECK(Step::interp_by_steps(parse_string("_if 1==2\n"
                                             "_then _true\n"
                                             "_else _false"))
                                             ->equals(NEW(BoolVal)(false)));
    CHECK_THROWS_WITH(Step::interp_by_steps(parse_string("_if _false\n"
                                                         "_then 0\n"
                                                         "_else y")),
                      "free variable: y");
    CHECK(Step::interp_by_steps(parse_string("_if (3+1)==(2+2)\n"
                                             "_then 25\n"
                                             "_else 100"))
                                             ->equals(NEW(NumVal)(25)));
    CHECK(Step::interp_by_steps(parse_string("_if _false\n"
                                             "_then -1\n"
                                             "_else _false"))
                  ->equals(NEW(BoolVal)(false)));
    CHECK_THROWS_WITH(Step::interp_by_steps(parse_string("_if 7\n"
                                                         "_then a+b\n"
                                                         "_else 0")),
                      "Cannot use is_true() on a NumVal");
}

TEST_CASE("LetExpr interp_by_steps() tests") {
    CHECK(Step::interp_by_steps(parse_string("_let x = 5\n"
                                             "_in  x+1"))->equals(NEW(NumVal)(6)));
    CHECK(Step::interp_by_steps(parse_string("_let x = 5+2\n"
                                             "_in  x+1"))->equals(NEW(NumVal)(8)));
    CHECK(Step::interp_by_steps(parse_string("5 * (_let x = 5\n"
                                             "    _in  x) + 1"))->equals(NEW(NumVal)(26)));
    CHECK(Step::interp_by_steps(parse_string("_let x = _let y = 6\n"
                                             "         _in  y*2\n"
                                             "_in  x+1"))->equals(NEW(NumVal)(13)));
}
