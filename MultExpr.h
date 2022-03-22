//
// Created by Kelan Albertson on 1/16/22.
//

#pragma once

#include "Expr.h"

class MultExpr : public Expr {
public:
    PTR(Expr) lhs_;
    PTR(Expr) rhs_;

    MultExpr(PTR(Expr) lhs, PTR(Expr) rhs);
    //returns whether the other Expr is an MultExpr and has all the same fields as this MultExpr
    virtual bool equals (PTR(Expr) other);
    //returns an integer interpretation of the MultExpr
    virtual PTR(Val) interp(PTR(Env) env);
    //returns whether this MultExpr contains a variable
//    virtual bool has_variable();
    //replaces every instance of a VarExpr matching the provided name in this MultExpr with a replacement Expr
    //if there is no matching VarExpr then nothing is replaced and a new Expr equal to this MultExpr is returned
//    virtual PTR(Expr) subst(std::string variableName, PTR(Expr) replacement);
    //prints the MultExpr
    virtual void print(std::ostream &out);
    //prints the expression with spaces around operators and without unnecessary parentheses
    //using an accumulator prec to determine which Expr need parentheses around them
    virtual void pretty_print_at(std::ostream &out, precedence_t prec, bool keyword_parentheses, std::streampos &last_new_line_pos);
};
