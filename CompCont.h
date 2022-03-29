//
// Created by Kelan Albertson on 3/28/22.
//

#pragma once

#include "Cont.h"

class Val;

class CompCont : public Cont {
public:
    PTR(Val) lhs_val_;
    PTR(Cont) rest_;

    CompCont(PTR(Val) lhs_val_, PTR(Cont) rest);
    // continues to the next step
    void step_continue();
};
