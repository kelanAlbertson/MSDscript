//
// Created by Kelan Albertson on 2/20/22.
//

#pragma once

#include "Val.h"

class BoolVal : public Val {
public:
    bool rep_;

    BoolVal (bool rep_);
    //returns whether the other Val is a BoolVal and has the same value as this BoolVal
    virtual bool equals(PTR(Val) other);
    // converts the Val to a human readable string
    virtual std::string to_string();
    // converts a Val to an Expr
//    virtual PTR(Expr) to_expr();
    // add a Val to a Val
    virtual PTR(Val) add_to(PTR(Val) other);
    // multiply a Val by another Val
    virtual PTR(Val) multiply_by(PTR(Val) other);
    // returns whether or not a Val is a BoolVal set to true
    virtual bool is_true();
    // calls a FunVal with the provided arg (raises an error for other Val types)
    virtual PTR(Val) call(PTR(Val) arg);
    // calls a FunVal with the provided arg but using the continuations
    virtual void call_step(PTR(Val) arg_val, PTR(Cont) rest);
};
