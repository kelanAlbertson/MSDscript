//
// Created by Kelan Albertson on 2/16/22.
//

#pragma once

#include "pointer.h"
#include <string>

class Expr;
class Cont;

CLASS(Val) {
public:
    // returns whether the Vals are the same type and hold the same value
    virtual bool equals(PTR(Val) other) = 0;
    // converts the Val to a human readable string
    virtual std::string to_string() = 0;
    // converts a Val to an Expr
    // no longer necessary so commenting out
//    virtual PTR(Expr) to_expr() = 0;
    // add a Val to a Val
    virtual PTR(Val) add_to(PTR(Val) other) = 0;
    // multiply a Val by a Val
    virtual PTR(Val) multiply_by(PTR(Val) other) = 0;
    // returns whether or not a Val is a BoolVal set to true
    virtual bool is_true() = 0;
    // calls a FunVal with the provided arg (raises an error for other Val types)
    virtual PTR(Val) call(PTR(Val) arg) = 0;
    // calls a FunVal with the provided arg but using the continuations
    virtual void call_step(PTR(Val) arg_val, PTR(Cont) rest) = 0;
};

