//
// Created by Kelan Albertson on 2/20/22.
//

#pragma once

#include "Expr.h"

class IfExpr : public Expr {
public:
    Expr *condition_;
    Expr *then_statement_;
    Expr *else_statement_;

    IfExpr (Expr *condition, Expr *ts, Expr *es);
    //returns whether the other Expr is an IfExpr and has all the same fields as this IfExpr
    virtual bool equals(Expr* other);
    //returns a Val representing the evaluation of this IfExpr
    virtual Val* interp();
    //returns whether this IfExpr contains a variable
    virtual bool has_variable();
    //replaces every instance of a VarExpr matching the provided name in this IfExpr with a replacement Expr
    //if there is no matching VarExpr then nothing is replaced and a new Expr equal to this IfExpr is returned
    virtual Expr* subst(std::string variableName, Expr* replacement);
    //prints this IfExpr
    virtual void print(std::ostream &out);
    //prints the expression with spaces around operators and without unnecessary parentheses
    //using an accumulator prec to determine which Expr need parentheses around them
    virtual void pretty_print_at(std::ostream &out, precedence_t prec, bool let_parentheses, std::streampos &last_new_line_pos);
};
