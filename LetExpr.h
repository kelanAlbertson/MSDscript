//
// Created by Kelan Albertson on 1/28/22.
//

#pragma once

#include "Expr.h"
#include "VarExpr.h"

class LetExpr : public Expr {
public:
    VarExpr* lhs_;
    Expr* rhs_;
    Expr* body_;

    LetExpr (VarExpr* lhs, Expr* rhs, Expr* body);
    //returns whether the other Expr is a LetExpr and has all the same fields as this LetExpr
    virtual bool equals (Expr* other);
    //returns an interpretation of the LetExpr
    virtual Val * interp();
    //returns whether this LetExpr contains a variable
    virtual bool has_variable();
    //replaces every instance of a VarExpr matching the provided name in this LetExpr with a replacement Expr
    //if there is no matching VarExpr then nothing is replaced and a new Expr equal to this LetExpr is returned
    virtual Expr* subst(std::string variableName, Expr* replacement);
    //prints the AddExpr
    virtual void print(std::ostream &out);
    //prints the expression with spaces around operators and without unnecessary parentheses
    //using an accumulator prec to determine which Expr need parentheses around them
    virtual void pretty_print_at(std::ostream &out, precedence_t prec, bool let_parentheses, std::streampos &last_new_line_pos);

};
