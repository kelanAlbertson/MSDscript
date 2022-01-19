//
// Created by Kelan Albertson on 1/16/22.
//

#pragma once

#include "expr.h"

class Num : public Expr {
public:
    int value_;

    Num (int value);
    //returns whether this Num is the same type and has the same value as the other Expr
    virtual bool equals (Expr* other);
    //returns an integer interpretation of the Num
    virtual int interp();
    //returns whether this Num contains a variable (this will always be false)
    virtual bool has_variable();
};
