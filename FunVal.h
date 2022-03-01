//
// Created by Kelan Albertson on 2/26/22.
//

#pragma once

#include "Val.h"
class VarExpr;

class FunVal : public Val {
public:
    VarExpr* arg_;
    Expr* body_;

    FunVal (VarExpr* arg, Expr* body);
    //returns whether the other Val is a FunVal and has the same value as this FunVal
    virtual bool equals(Val* other);
    // converts the Val to a human readable string
    virtual std::string to_string();
    // converts a Val to an Expr
    virtual Expr* to_expr();
    // add a Val to a Val
    virtual Val* add_to(Val* other);
    // multiply a Val by another Val
    virtual Val* multiply_by(Val* other);
    // returns whether or not a Val is a BoolVal set to true
    virtual bool is_true();
    // calls a FunVal with the provided arg (raises an error for other Val types)
    virtual Val* call(Val* arg);
};
