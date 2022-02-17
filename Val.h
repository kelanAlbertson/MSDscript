//
// Created by Kelan Albertson on 2/16/22.
//

#pragma once

#include <string>

class Expr;

class Val {
public:
    // returns whether the Vals are the same type and hold the same value
    virtual bool equals(Val* other) = 0;
    // converts the Val to a human readable string
    virtual std::string to_string() = 0;
    // converts a Val to an Expr
    virtual Expr* to_expr() = 0;
    // add a Val to a Val
    virtual Val* add_to(Val* other) = 0;
    // multiply a Val by a Val
    virtual Val* multiply_by(Val* other) = 0;
};

