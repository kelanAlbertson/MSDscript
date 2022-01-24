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
    //replaces every instance of a Var matching the provided name in this Expr with a replacement Expr
    //but since there cannot be a Var within a Num, this will always return a new Num equal to this Num
    virtual Expr* subst(std::string variableName, Expr* replacement);
    //prints the Num
    virtual void print(std::ostream &out);
};
