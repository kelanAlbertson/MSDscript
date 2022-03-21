//
// Created by Kelan Albertson on 3/20/22.
//

//TODO:
//     X adding an Env class
//     X adding PTR(Env) as an argument to Expr::interp
//     X removing the Expr::subst method
//      adapting existing Expr::interp tests.

#pragma once

#include "pointer.h"

class Val;

CLASS(Env) {
public:
    static PTR(Env) empty;
    // lookup a variable within this environment to see if there is a value to replace it with
    virtual PTR(Val) lookup(std::string find_name) = 0;
};
