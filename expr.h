//
// Created by Kelan Albertson on 1/16/22.
//

#pragma once

#include <string>

class Expr {
public:
    //returns whether this Expr has all the same fields as the other Expr
    virtual bool equals(Expr* other) = 0;
    //returns an integer interpretation of the Expr (if possible)
    virtual int interp() = 0;
    //returns whether this Expr is or contains a variable
    virtual bool has_variable() = 0;
    //replaces every instance in the Expr of a Var matching the provided name with a replacement Expr
    virtual Expr* subst(std::string variableName, Expr* replacement) = 0;
    //prints the expression
    virtual void print(std::ostream &out) = 0;
    //returns the string representation of the Expr using the print() function
    std::string to_string();
    //prints the expression with spaces around operators and without unnecessary parentheses
    virtual void pretty_print(std::ostream &out);
    typedef enum {
        prec_none,      // = 0
        prec_add,       // = 1
        prec_mult       // = 2
    } precedence_t;
    //pretty_print using an accumulator prec to determine which Expr need parentheses around them
    virtual void pretty_print_at(std::ostream &out, precedence_t prec, bool let_parentheses, std::streampos &last_new_line_pos) = 0;
    //returns the string representation of the Expr using the pretty_print() function
    std::string to_pretty_string();
};
