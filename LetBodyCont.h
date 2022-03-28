//
// Created by Kelan Albertson on 3/28/22.
//

#pragma once

#include <string>
#include "Cont.h"

class Expr;
class Env;

class LetBodyCont : public Cont {
public:
    std::string var_;
    PTR(Expr) body_;
    PTR(Env) env_;
    PTR(Cont) rest_;

    LetBodyCont(std::string var, PTR(Expr) body, PTR(Env) env, PTR(Cont) rest);

    virtual void step_continue();
};
