//
// Created by Kelan Albertson on 3/28/22.
//

#include "RightThenCompCont.h"
#include "Val.h"
#include "Step.h"

RightThenCompCont::RightThenCompCont(PTR(Expr) rhs, PTR(Env) env, PTR(Cont) rest) {
    this->rhs_ = rhs;
    this->env_ = env;
    this->rest_ = rest;
}

void RightThenCompCont::step_continue() {
    PTR(Val) lhs_val = Step::val_;
    Step::mode_ = Step::interp_mode;
    Step::expr_ = rhs_;
    Step::env_ = this->env_;
    Step::cont_ = NEW(CompCont) (lhs_val, rest_);
}