//
// Created by Kelan Albertson on 3/28/22.
//

#include "Step.h"
#include "Env.h"

int Step::interp_by_steps(PTR(Expr) e) {
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
