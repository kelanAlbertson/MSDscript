//
// Created by Kelan Albertson on 3/28/22.
//

#include "ArgThenCallCont.h"
#include "Step.h"
#include "CallCont.h"

ArgThenCallCont::ArgThenCallCont(PTR(Expr) arg, PTR(Env) env, PTR(Cont) rest) {
    this->arg_ = arg;
    this->env_ = env;
    this->rest_ = rest;
}

void ArgThenCallCont::step_continue() {
    Step::mode_ = Step::interp_mode;
    Step::expr_ = arg_;
    Step::env_ = this->env_;
    Step::cont_ = NEW(CallCont)(Step::val_, rest_);
}