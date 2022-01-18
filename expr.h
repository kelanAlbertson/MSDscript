//
// Created by Kelan Albertson on 1/16/22.
//

#pragma once

class Expr {
    virtual bool equals(Expr* other) = 0;
};

//class Num : public Expr {
//public:
//    int value_;
//
//    Num (int value);
//    virtual bool equals (Expr* other);
//};
//
//class Add : public Expr {
//public:
//    Expr* lhs_;
//    Expr* rhs_;
//
//    Add (Expr* lhs, Expr* rhs);
//    virtual equals (Expr* other);
//};
//
//class Mult : public Expr {
//public:
//    Expr* lhs_;
//    Expr* rhs_;
//
//    Mult(Expr* lhs, Expr* rhs);
//    virtual bool equals (Expr* other);
//};
//
//class Var : public Expr {
//public:
//    std::string name_;
//    int value_;
//};
