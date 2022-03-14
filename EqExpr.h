//
// Created by Kelan Albertson on 2/20/22.
//

#pragma once

#include "Expr.h"

class EqExpr : public Expr {
public:
    Expr *lhs_;
    Expr *rhs_;

    EqExpr (Expr *lhs, Expr *rhs);
    //returns whether the other Expr is an EqExpr and has all the same fields as this EqExpr
    virtual bool equals(Expr* other);
    //returns a Val representing the evaluation of this EqExpr
    virtual Val* interp();
    //returns whether this EqExpr contains a variable
//    virtual bool has_variable();
    //replaces every instance of a VarExpr matching the provided name in this EqExpr with a replacement Expr
    //if there is no matching VarExpr then nothing is replaced and a new Expr equal to this EqExpr is returned
    virtual Expr* subst(std::string variableName, Expr* replacement);
    //prints this EqExpr
    virtual void print(std::ostream &out);
    //prints the expression with spaces around operators and without unnecessary parentheses
    //using an accumulator prec to determine which Expr need parentheses around them
    virtual void pretty_print_at(std::ostream &out, precedence_t prec, bool keyword_parentheses, std::streampos &last_new_line_pos);
};

