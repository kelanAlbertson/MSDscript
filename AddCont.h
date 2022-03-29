//
// Created by Kelan Albertson on 3/28/22.
//

#pragma once

#include "Cont.h"

class Val;

class AddCont : public Cont {
public:
    PTR(Val) lhs_val_;
    PTR(Cont) rest_;

    AddCont(PTR(Val) lhs_val_, PTR(Cont) rest);
    // continues to the next step
    void step_continue();
};
