//
// Created by Kelan Albertson on 1/17/22.
//

#pragma once

class Var : public Expr {
public:
    std::string name_;
    int value_;

    Var(std::string name, int value);
    virtual bool equals(Expr* other);
};

