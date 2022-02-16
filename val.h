//
// Created by Kelan Albertson on 2/16/22.
//

#pragma once

class Expr;

class val {
public:
    // converts a Val to an Expr
    virtual Expr* to_expr() = 0;

};

