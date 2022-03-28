//
// Created by Kelan Albertson on 3/28/22.
//

#pragma once

#include "pointer.h"

class Expr;
class Env;
class Val;
class Cont;

class Step {
public:
    typedef enum {
        interp_mode,
        continue_mode
    } mode_t;
    static mode_t mode_;        // chooses mode
    static PTR(Expr) expr_;     // for interp mode
    static PTR(Env) env_;       // for interp mode
    static PTR(Val) val_;       // for continue mode
    static PTR(Cont) cont_;     // all modes

    PTR(Val) interp_by_steps(PTR(Expr) e);
};
