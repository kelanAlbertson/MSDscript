//
// Created by Kelan Albertson on 3/28/22.
//

#pragma once

#include "Cont.h"

class Expr;
class Env;

class IfBranchCont : public Cont {
public:
    PTR(Expr) then_statement_;
    PTR(Expr) else_statement_;
    PTR(Env) env_;
    PTR(Cont) rest_;

    IfBranchCont(PTR(Expr) then_statement, PTR(Expr) else_statement, PTR(Env) env, PTR(Cont) rest);

    virtual void step_continue();
};
