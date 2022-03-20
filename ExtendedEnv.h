//
// Created by Kelan Albertson on 3/20/22.
//

#pragma once

#include "Env.h"

class ExtendedEnv : public Env {
public:
    std::string name_;
    PTR(Val) val_;
    PTR(Env) rest_;

    // lookup a variable within this environment to see if there is a value to replace it with
    virtual PTR(Val) lookup(std::string find_name);
};
