//
// Created by Kelan Albertson on 3/20/22.
//

#pragma once

#include "pointer.h"

class Val;

CLASS(Env) {
public:
    static PTR(Env) empty;
    // lookup a variable within this environment to see if there is a value to replace it with
    virtual PTR(Val) lookup(std::string find_name) = 0;
    // returns whether this Env has all the same fields as the other Env
    virtual bool equals(PTR(Env) other) = 0;
};
