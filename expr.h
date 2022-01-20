//
// Created by Kelan Albertson on 1/16/22.
//

#pragma once

#include <string>

class Expr {
public:
    //returns whether this Expr has all the same fields as the other Expr
    virtual bool equals(Expr* other) = 0;
    //returns an integer interpretation of the Expr (if possible)
    virtual int interp() = 0;
    //returns whether this Expr is or contains a variable
    virtual bool has_variable() = 0;
    //replaces every instance in the Expr of a Var matching the provided name with a replacement Expr
    virtual Expr* subst(std::string variableName, Expr* replacement) = 0;
};
