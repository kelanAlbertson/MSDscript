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
    virtual bool equals (Expr* other);
};
