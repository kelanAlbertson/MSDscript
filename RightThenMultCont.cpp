//
// Created by Kelan Albertson on 3/28/22.
//

#include "RightThenMultCont.h"
#include "Val.h"
#include "Step.h"
#include "MultCont.h"

RightThenMultCont::RightThenMultCont(PTR(Expr) rhs, PTR(Env) env, PTR(Cont) rest) {
    this->rhs_ = rhs;
    this->env_ = env;
    this->rest_ = rest;
}

void RightThenMultCont::step_continue() {
    PTR(Val) lhs_val = Step::val_;
    Step::mode_ = Step::interp_mode;
    Step::expr_ = rhs_;
    Step::env_ = this->env_;
    Step::cont_ = NEW(MultCont) (lhs_val, rest_);
}