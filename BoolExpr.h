//
// Created by Kelan Albertson on 2/20/22.
//

#pragma once

#include "Expr.h"

class BoolExpr : public Expr {
public:
    int rep_;

    BoolExpr (int rep);
    //returns whether the other Expr is a BoolExpr and has the same value as this BoolExpr
    virtual bool equals (PTR(Expr) other);
    //returns a Val representation of this BoolExpr
    virtual PTR(Val) interp();
    //returns whether this BoolExpr contains a variable (this will always be false)
//    virtual bool has_variable();
    //replaces every instance of a VarExpr matching the provided name in this Expr with a replacement Expr
    //but since there cannot be a VarExpr within a BoolExpr, this will always return a new BoolExpr equal to this BoolExpr
    virtual PTR(Expr) subst(std::string variableName, PTR(Expr) replacement);
    //prints the BoolExpr
    virtual void print(std::ostream &out);
    //prints the expression with spaces around operators and without unnecessary parentheses
    //using an accumulator prec to determine which Expr need parentheses around them
    virtual void pretty_print_at(std::ostream &out, precedence_t prec, bool keyword_parentheses, std::streampos &last_new_line_pos);
};
