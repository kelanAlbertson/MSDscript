//
// Created by Kelan Albertson on 3/28/22.
//

#pragma once

#include "Cont.h"

class Val;

class CallCont : public Cont {
public:
    PTR(Val) to_be_called_val_;
    PTR(Cont) rest_;

    CallCont(PTR(Val) to_be_called_val, PTR(Cont) rest);

    virtual void step_continue();
};
