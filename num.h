//
// Created by Kelan Albertson on 1/16/22.
//

#pragma once

#include "expr.h"

class Num : public Expr {
public:
    int rep_;

    Num (int value);
    //returns whether the other Expr is a Num and has the same value as this Num
    virtual bool equals (Expr* other);
    //returns an integer interpretation of the Num
    virtual Val * interp();
    //returns whether this Num contains a variable (this will always be false)
    virtual bool has_variable();
    //replaces every instance of a Var matching the provided name in this Expr with a replacement Expr
    //but since there cannot be a Var within a Num, this will always return a new Num equal to this Num
    virtual Expr* subst(std::string variableName, Expr* replacement);
    //prints the Num
    virtual void print(std::ostream &out);
    //prints the expression with spaces around operators and without unnecessary parentheses
    //using an accumulator prec to determine which Expr need parentheses around them
    virtual void pretty_print_at(std::ostream &out, precedence_t prec, bool let_parentheses, std::streampos &last_new_line_pos);
};
