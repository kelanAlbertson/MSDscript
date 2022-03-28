//
// Created by Kelan Albertson on 3/28/22.
//

#pragma once

#include "Cont.h"

class Expr;
class Env;

class ArgThenCallCont : public Cont {
public:
    PTR(Expr) arg_;
    PTR(Env) env_;
    PTR(Cont) rest_;

    ArgThenCallCont(PTR(Expr) arg, PTR(Env) env, PTR(Cont) rest);

    virtual void step_continue();
};
