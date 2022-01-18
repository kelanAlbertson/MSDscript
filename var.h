//
// Created by Kelan Albertson on 1/17/22.
//

#pragma once

#include <string>
#include "expr.h"

class Var : public Expr {
public:
    std::string name_;

    Var(std::string name);
    //returns whether this Expr has all the same fields as the other Expr
    virtual bool equals(Expr* other);
    //returns an integer interpretation of the Var
    virtual int interp();
};

