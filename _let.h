//
// Created by Kelan Albertson on 1/28/22.
//

#pragma once

#include "expr.h"
#include "var.h"

class _let : public Expr {
public:
    Var* lhs_;
    Expr* rhs_;
    Expr* body_;

    _let (Var* lhs, Expr* rhs, Expr* body);
    //returns whether the other Expr is a _let and has all the same fields as this _let
    virtual bool equals (Expr* other);
    //returns an interpretation of the _let
    virtual int interp();
    //returns whether this _let contains a variable
    virtual bool has_variable();
    //replaces every instance of a Var matching the provided name in this _let with a replacement Expr
    //if there is no matching Var then nothing is replaced and a new Expr equal to this _let is returned
    virtual Expr* subst(std::string variableName, Expr* replacement);
    //prints the Add
    virtual void print(std::ostream &out);
    //prints the expression with spaces around operators and without unnecessary parentheses
    //using an accumulator prec to determine which Expr need parentheses around them
    virtual void pretty_print_at(std::ostream &out, precedence_t prec, bool let_parentheses, std::streampos &last_new_line_pos);

};
