//
// Created by Kelan Albertson on 3/20/22.
//

#pragma once

#include <string>
#include "Env.h"

class ExtendedEnv : public Env {
public:
    std::string name_;
    PTR(Val) val_;
    PTR(Env) rest_;

    ExtendedEnv(std::string name, PTR(Val) val, PTR(Env) rest);
    // returns whether the other Env is an ExtendedEnv with all the same member variables as this ExtendedEnv
    virtual bool equals(PTR(Env) other);
    // lookup a variable within this environment to see if there is a value to replace it with
    virtual PTR(Val) lookup(std::string find_name);
};
