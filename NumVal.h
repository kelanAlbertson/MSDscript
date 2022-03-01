//
// Created by Kelan Albertson on 2/16/22.
//

#pragma once

#include "Val.h"

class NumVal : public Val {
public:
    int rep_;

    NumVal (int rep);
    //returns whether the other Val is a NumVal and has the same value as this NumVal
    virtual bool equals(Val* other);
    // converts the Val to a human readable string
    virtual std::string to_string();
    // converts a Val to an Expr
    virtual Expr* to_expr();
    // add a Val to a Val
    virtual Val* add_to(Val* other);
    // multiply a Val by another Val
    virtual Val* multiply_by(Val* other);
    // will always throw an error because is_true will only work on BoolVals
    virtual bool is_true();
    // calls a FunVal with the provided arg (raises an error for other Val types)
    virtual Val* call(Val* arg);
};

