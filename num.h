//
// Created by Kelan Albertson on 1/16/22.
//

#pragma once

#include "expr.h"

class Num : public Expr {
public:
    int value_;

    Num (int value);
    virtual bool equals (Expr* other);
};
