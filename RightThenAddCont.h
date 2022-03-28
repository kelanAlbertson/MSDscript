//
// Created by Kelan Albertson on 3/28/22.
//

#pragma once

#include "Cont.h"

class Expr;
class Env;

class RightThenAddCont : public Cont {
public:
    PTR(Expr) rhs_;
    PTR(Env) env_;
    PTR(Cont) rest_;

    RightThenAddCont(PTR(Expr) rhs, PTR(Env) env, PTR(Cont) rest);
    // continues to the next step
    void step_continue();
};
