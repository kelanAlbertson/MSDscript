//
// Created by Kelan Albertson on 1/16/22.
//

#pragma once

class Expr {
public:
    virtual bool equals(Expr* other) = 0;
};
