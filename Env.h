//
// Created by Kelan Albertson on 3/20/22.
//

//TODO:
//adding an Env class
//adding PTR(Env) as an argument to Expr::interp
//removing the Expr::subst method
//adapting existing Expr::interp tests.

#pragma once

#include "pointer.h"

class Val;

CLASS(Env) {
public:
    // lookup a variable within this environment to see if there is a value to replace it with
    virtual PTR(Val) lookup(std::string find_name) = 0;
};
