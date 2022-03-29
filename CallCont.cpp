//
// Created by Kelan Albertson on 3/28/22.
//

#include "CallCont.h"
#include "Val.h"
#include "Step.h"

CallCont::CallCont(PTR(Val) to_be_called_val, PTR(Cont) rest) {
   this->to_be_called_val_ = to_be_called_val;
   this->rest_ = rest;
}

void CallCont::step_continue() {
    to_be_called_val_->call_step(Step::val_, rest_);
}
