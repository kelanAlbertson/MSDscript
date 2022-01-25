//
// Created by Kelan Albertson on 1/17/22.
//

#pragma once

#include "expr.h"

class Var : public Expr {
public:
    std::string name_;

    Var(std::string name);
    //returns whether this Expr has all the same fields as the other Expr
    virtual bool equals(Expr* other);
    //returns an integer interpretation of the Var
    virtual int interp();
    //returns whether this Var is a variable (this will always be true)
    virtual bool has_variable();
    //if the variable name matches the name of this Var, then replaces this Var with the provided replacement Expr
    //if the variable does not match the name of this Var, then a new Var equal to this Var is returned
    virtual Expr* subst(std::string variableName, Expr* replacement);
    //prints the Var
    virtual void print(std::ostream &out);
    //prints the expression with spaces around operators and without unnecessary parentheses
    //using an accumulator prec to determine which Expr need parentheses around them
    virtual void pretty_print_at(std::ostream &out, precedence_t prec);
};

