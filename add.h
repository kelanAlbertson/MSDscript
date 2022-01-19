//
// Created by Kelan Albertson on 1/16/22.
//

#pragma once

#include "expr.h"

class Add : public Expr {
public:
    Expr* lhs_;
    Expr* rhs_;

    Add (Expr* lhs, Expr* rhs);
    //returns whether this Expr has all the same fields as the other Expr
    virtual bool equals (Expr* other);
    //returns an integer interpretation of the Add
    virtual int interp();
    //returns whether this Add contains a variable
    virtual bool has_variable();
};
