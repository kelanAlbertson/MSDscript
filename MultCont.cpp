//
// Created by Kelan Albertson on 3/28/22.
//

#include "MultCont.h"
#include "Step.h"
#include "Val.h"

MultCont::MultCont(PTR(Val) lhs_val, PTR(Cont) rest) {
    this->lhs_val_ = lhs_val;
    this->rest_ = rest;
}

void MultCont::step_continue() {
    PTR(Val) rhs_val = Step::val_;
    Step::mode_ = Step::continue_mode;
    Step::val_ = lhs_val_->multiply_by(rhs_val);
    Step::cont_ = rest_;
}