//
// Created by Kelan Albertson on 2/26/22.
//

#include "FunVal.h"
#include "VarExpr.h"
#include "catch.h"
#include "AddExpr.h"
#include "NumExpr.h"
#include "FunExpr.h"
#include "BoolVal.h"
#include "NumVal.h"
#include "BoolExpr.h"

FunVal::FunVal(VarExpr *arg, Expr *body) {
    this->arg_ = arg;
    this->body_ = body;
}

bool FunVal::equals(Val *other) {
    FunVal *fv = dynamic_cast<FunVal*>(other);
    if (fv == nullptr) {
        return false;
    }
    else {
        return (this->arg_->equals(fv->arg_) && this->body_->equals(fv->body_));
    }
}

std::string FunVal::to_string() {
    return "(_fun (" + this->arg_->to_string() + ") " + this->body_->to_string() + ")";
}

Expr *FunVal::to_expr() {
    return new FunExpr(this->arg_, this->body_);
}

Val *FunVal::add_to(Val *other) {
    throw std::runtime_error("Cannot add_to() with a FunVal");
}

Val *FunVal::multiply_by(Val *other) {
    throw std::runtime_error("Cannot multiply_by() with a FunVal");
}

bool FunVal::is_true() {
    throw std::runtime_error("Cannot use is_true() on a FunVal");
}

Val* FunVal::call(Val* arg) {
    return this->body_->subst(this->arg_->name_, arg->to_expr())->interp();
}

/**
 *************************   TESTS   **************************
 **/

TEST_CASE("FunVal equals() tests") {
    CHECK((new FunVal(new VarExpr("x"), new AddExpr(new VarExpr("x"), new NumExpr(1))))
                  ->equals(new FunVal(new VarExpr("x"), new AddExpr(new VarExpr("x"), new NumExpr(1)))) == true);
    CHECK((new FunVal(new VarExpr("x"), new AddExpr(new VarExpr("x"), new NumExpr(1))))
                  ->equals(new FunVal(new VarExpr("x"), new NumExpr(1))) == false);
    CHECK((new FunVal(new VarExpr("x"), new AddExpr(new VarExpr("x"), new NumExpr(1))))
                  ->equals(new BoolVal(true)) == false);
}

TEST_CASE("FunVal to_string() tests") {
    CHECK((new FunVal(new VarExpr("x"), new NumExpr(1)))->to_string() == "(_fun (x) 1)");
    CHECK((new FunVal(new VarExpr("x"), new AddExpr(new VarExpr("x"), new NumExpr(1))))
                  ->to_string() == "(_fun (x) (x+1))");
    CHECK((new FunVal(new VarExpr("x"), new VarExpr("y")))->to_string() == "(_fun (x) y)");
}

TEST_CASE("FunVal to_expr() tests") {
    CHECK((new FunVal(new VarExpr("y"), new VarExpr("y")))->to_expr()
            ->equals(new FunExpr(new VarExpr("y"), new VarExpr("y"))));
    CHECK((new FunVal(new VarExpr("y"), new AddExpr(new VarExpr("y"), new NumExpr(1))))->to_expr()
                  ->equals(new FunExpr(new VarExpr("y"), new AddExpr(new VarExpr("y"), new NumExpr(1)))));
}

TEST_CASE("FunVal add_to() tests") {
    CHECK_THROWS_WITH((new FunVal(new VarExpr("y"), new VarExpr("y")))->add_to(new NumVal(1)), "Cannot add_to() with a FunVal");
}

TEST_CASE("FunVal multiply_by() tests") {
    CHECK_THROWS_WITH((new FunVal(new VarExpr("y"), new VarExpr("y")))->multiply_by(new NumVal(1)), "Cannot multiply_by() with a FunVal");
}

TEST_CASE("FunVal is_true() tests") {
    CHECK_THROWS_WITH((new FunVal(new VarExpr("x"), new NumExpr(1)))->is_true(), "Cannot use is_true() on a FunVal");
}

TEST_CASE("FunVal call() tests") {
    CHECK((new FunVal(new VarExpr("x"), new NumExpr(1)))->call(new NumVal(5))->equals(new NumVal(1)));
    CHECK((new FunVal(new VarExpr("x"), new AddExpr(new VarExpr("x"), new NumExpr(1))))->call(new NumVal(5))->equals(new NumVal(6)));
    CHECK((new FunVal(new VarExpr("x"), new BoolExpr(true)))->call(new NumVal(5))->equals(new BoolVal(true)));
    CHECK_THROWS_WITH((new FunVal(new VarExpr("x"), new AddExpr(new VarExpr("x"), new BoolExpr(true))))
            ->call(new NumVal(5))->equals(new BoolVal(true)), "Cannot use add_to() with a non-number");
}
