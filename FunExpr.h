//
// Created by Kelan Albertson on 2/26/22.
//

#pragma once

#include "Expr.h"
class VarExpr;

class FunExpr : public Expr {
public:
    PTR(VarExpr) arg_;
    PTR(Expr) body_;

    FunExpr (PTR(VarExpr) arg, PTR(Expr)body);
    //returns whether the other Expr is a FunExpr and has all the same fields as this FunExpr
    virtual bool equals(PTR(Expr) other);
    //returns a Val representing the evaluation of this FunExpr
    virtual PTR(Val) interp(PTR(Env) env);
    //returns whether this FunExpr contains a variable
//    virtual bool has_variable();
    //replaces every instance of a VarExpr matching the provided name in this FunExpr with a replacement Expr
    //if there is no matching VarExpr then nothing is replaced and a new Expr equal to this FunExpr is returned
    virtual PTR(Expr) subst(std::string variableName, PTR(Expr) replacement);
    //prints this FunExpr
    virtual void print(std::ostream &out);
    //prints the expression with spaces around operators and without unnecessary parentheses
    //using an accumulator prec to determine which Expr need parentheses around them
    virtual void pretty_print_at(std::ostream &out, precedence_t prec, bool keyword_parentheses, std::streampos &last_new_line_pos);
};
