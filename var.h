//
// Created by Kelan Albertson on 1/17/22.
//

#pragma once

#include <string>
#include "expr.h"

class Var : public Expr {
public:
    std::string name_;

    Var(std::string name);
    virtual bool equals(Expr* other);
};

