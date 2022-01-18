//
// Created by Kelan Albertson on 1/16/22.
//

#pragma once

class Expr {
public:
    //returns whether this Expr has all the same fields as the other Expr
    virtual bool equals(Expr* other) = 0;
    //returns an integer interpretation of the Expr (if possible)
    virtual int interp() = 0;
};
