//
// Created by Kelan Albertson on 3/28/22.
//

#include "IfBranchCont.h"
#include "Step.h"
#include "Val.h"

IfBranchCont::IfBranchCont(PTR(Expr) then_statement, PTR(Expr) else_statement, PTR(Env) env, PTR(Cont) rest) {
    this->then_statement_ = then_statement;
    this->else_statement_ = else_statement;
    this->env_ = env;
    this->rest_ = rest;
}

void IfBranchCont::step_continue() {
    PTR(Val) test_val = Step::val_;
    Step::mode_ = Step::interp_mode;
    if (test_val->is_true()) {
        Step::expr_ = then_statement_;
    }
    else {
        Step::expr_ = else_statement_;
    }
    Step::env_ = this->env_;
    Step::cont_ = rest_;
}
