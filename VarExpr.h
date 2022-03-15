//
// Created by Kelan Albertson on 1/17/22.
//

#pragma once

#include "Expr.h"

class VarExpr : public Expr {
public:
    std::string name_;

    VarExpr(std::string name);
    //returns whether the other Expr is a VarExpr and has the same name as this VarExpr
    virtual bool equals(PTR(Expr) other);
    //returns an integer interpretation of the VarExpr
    virtual PTR(Val) interp();
    //returns whether this VarExpr is a variable (this will always be true)
//    virtual bool has_variable();
    //if the variable name matches the name of this VarExpr, then replaces this VarExpr with the provided replacement Expr
    //if the variable does not match the name of this VarExpr, then a new VarExpr equal to this VarExpr is returned
    virtual PTR(Expr) subst(std::string variableName, PTR(Expr) replacement);
    //prints the VarExpr
    virtual void print(std::ostream &out);
    //prints the expression with spaces around operators and without unnecessary parentheses
    //using an accumulator prec to determine which Expr need parentheses around them
    virtual void pretty_print_at(std::ostream &out, precedence_t prec, bool keyword_parentheses, std::streampos &pos);
};

