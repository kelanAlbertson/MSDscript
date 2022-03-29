//
// Created by Kelan Albertson on 3/28/22.
//

#include "RightThenAddCont.h"
#include "Val.h"
#include "Step.h"
#include "AddCont.h"

RightThenAddCont::RightThenAddCont(PTR(Expr) rhs, PTR(Env) env, PTR(Cont) rest) {
    this->rhs_ = rhs;
    this->env_ = env;
    this->rest_ = rest;
}

void RightThenAddCont::step_continue() {
    PTR(Val) lhs_val = Step::val_;
    Step::mode_ = Step::interp_mode;
    Step::expr_ = rhs_;
    Step::env_ = this->env_;
    Step::cont_ = NEW(AddCont) (lhs_val, rest_);
}
