//
// Created by Kelan Albertson on 1/16/22.
//

#pragma once

#include "expr.h"

class Mult : public Expr {
public:
    Expr* lhs_;
    Expr* rhs_;

    Mult(Expr* lhs, Expr* rhs);
    //returns whether this Expr has all the same fields as the other Expr
    virtual bool equals (Expr* other);
    //returns an integer interpretation of the Mult
    virtual int interp();
    //returns whether this Mult contains a variable
    virtual bool has_variable();
    //replaces every instance of a Var matching the provided name in this Mult with a replacement Expr
    //if there is no matching Var then nothing is replaced and a new Expr equal to this Mult is returned
    virtual Expr* subst(std::string variableName, Expr* replacement);
    //prints the Mult
    virtual void print(std::ostream &out);
    //prints the expression with spaces around operators and without unnecessary parentheses
    //using an accumulator prec to determine which Expr need parentheses around them
    virtual void pretty_print_at(std::ostream &out, precedence_t prec);
};
