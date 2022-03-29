//
// Created by Kelan Albertson on 1/16/22.
//

#pragma once

#include "Expr.h"

class NumExpr : public Expr {
public:
    int rep_;

    NumExpr (int rep);
    //returns whether the other Expr is a NumExpr and has the same value as this NumExpr
    virtual bool equals (PTR(Expr) other);
    //returns an integer interpretation of the NumExpr
    virtual PTR(Val) interp(PTR(Env) env);
    //interprets this NumExpr using continuations
    virtual void step_interp();
    //returns whether this NumExpr contains a variable (this will always be false)
//    virtual bool has_variable();
    //replaces every instance of a VarExpr matching the provided name in this Expr with a replacement Expr
    //but since there cannot be a VarExpr within a NumExpr, this will always return a new NumExpr equal to this NumExpr
//    virtual PTR(Expr) subst(std::string variableName, PTR(Expr) replacement);
    //prints the NumExpr
    virtual void print(std::ostream &out);
    //prints the expression with spaces around operators and without unnecessary parentheses
    //using an accumulator prec to determine which Expr need parentheses around them
    virtual void pretty_print_at(std::ostream &out, precedence_t prec, bool keyword_parentheses, std::streampos &last_new_line_pos);
};
