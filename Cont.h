//
// Created by Kelan Albertson on 3/28/22.
//

#pragma once
#include "pointer.h"

CLASS(Cont) {
public:
    static PTR(Cont) done;
    virtual void step_continue() = 0;
};
