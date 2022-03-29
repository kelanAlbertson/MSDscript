//
// Created by Kelan Albertson on 3/28/22.
//

#include "CompCont.h"
#include "Step.h"
#include "Val.h"
#include "BoolVal.h"

CompCont::CompCont(PTR(Val) lhs_val, PTR(Cont) rest) {
    this->lhs_val_ = lhs_val;
    this->rest_ = rest;
}

void CompCont::step_continue() {
    PTR(Val) rhs_val = Step::val_;
    Step::mode_ = Step::continue_mode;
    Step::val_ = NEW(BoolVal)(lhs_val_->equals(rhs_val));
    Step::cont_ = rest_;
}