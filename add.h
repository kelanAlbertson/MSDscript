//
// Created by Kelan Albertson on 1/16/22.
//

#pragma once

#include "expr.h"

class Add : public Expr {
public:
    Expr* lhs_;
    Expr* rhs_;

    Add (Expr* lhs, Expr* rhs);
    //returns whether the other Expr is an Add and has all the same fields as this Add
    virtual bool equals (Expr* other);
    //returns an integer interpretation of the Add
    virtual Val * interp();
    //returns whether this Add contains a variable
    virtual bool has_variable();
    //replaces every instance of a Var matching the provided name in this Add with a replacement Expr
    //if there is no matching Var then nothing is replaced and a new Expr equal to this Add is returned
    virtual Expr* subst(std::string variableName, Expr* replacement);
    //prints the Add
    virtual void print(std::ostream &out);
    //prints the expression with spaces around operators and without unnecessary parentheses
    //using an accumulator prec to determine which Expr need parentheses around them
    virtual void pretty_print_at(std::ostream &out, precedence_t prec, bool let_parentheses, std::streampos &last_new_line_pos);
};
