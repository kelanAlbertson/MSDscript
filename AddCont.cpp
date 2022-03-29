//
// Created by Kelan Albertson on 3/28/22.
//

#include "AddCont.h"
#include "Step.h"
#include "Val.h"

AddCont::AddCont(PTR(Val) lhs_val, PTR(Cont) rest) {
    this->lhs_val_ = lhs_val;
    this->rest_ = rest;
}

void AddCont::step_continue() {
    PTR(Val) rhs_val = Step::val_;
    Step::mode_ = Step::continue_mode;
    Step::val_ = lhs_val_->add_to(rhs_val);
    Step::cont_ = rest_;
}
