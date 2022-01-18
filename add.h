//
// Created by Kelan Albertson on 1/16/22.
//

#pragma once

class Add : public Expr {
public:
    Expr* lhs_;
    Expr* rhs_;

    Add (Expr* lhs, Expr* rhs);
    virtual equals (Expr* other);
};
