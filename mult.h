//
// Created by Kelan Albertson on 1/16/22.
//

#pragma once

#include "expr.h"

class Mult : public Expr {
public:
    Expr* lhs_;
    Expr* rhs_;

    Mult(Expr* lhs, Expr* rhs);
    //returns whether this Expr has all the same fields as the other Expr
    virtual bool equals (Expr* other);
    //returns an integer interpretation of the Mult
    virtual int interp();
};
