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

FunVal::FunVal(PTR(VarExpr) arg, PTR(Expr) body) {
    this->arg_ = arg;
    this->body_ = body;
}

bool FunVal::equals(PTR(Val)other) {
    PTR(FunVal)fv = CAST(FunVal)(other);
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

PTR(Expr)FunVal::to_expr() {
    return NEW(FunExpr)(this->arg_, this->body_);
}

PTR(Val)FunVal::add_to(PTR(Val) other) {
    throw std::runtime_error("Cannot add_to() with a FunVal");
}

PTR(Val)FunVal::multiply_by(PTR(Val) other) {
    throw std::runtime_error("Cannot multiply_by() with a FunVal");
}

bool FunVal::is_true() {
    throw std::runtime_error("Cannot use is_true() on a FunVal");
}

PTR(Val) FunVal::call(PTR(Val) arg) {
    return this->body_->subst(this->arg_->name_, arg->to_expr())->interp();
}

/**
 *************************   TESTS   **************************
 **/

TEST_CASE("FunVal equals() tests") {
    CHECK((NEW(FunVal)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))
                  ->equals(NEW(FunVal)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))) == true);
    CHECK((NEW(FunVal)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))
                  ->equals(NEW(FunVal)(NEW(VarExpr)("x"), NEW(NumExpr)(1))) == false);
    CHECK((NEW(FunVal)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))
                  ->equals(NEW(BoolVal)(true)) == false);
}

TEST_CASE("FunVal to_string() tests") {
    CHECK((NEW(FunVal)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))->to_string() == "(_fun (x) 1)");
    CHECK((NEW(FunVal)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))
                  ->to_string() == "(_fun (x) (x+1))");
    CHECK((NEW(FunVal)(NEW(VarExpr)("x"), NEW(VarExpr)("y")))->to_string() == "(_fun (x) y)");
}

TEST_CASE("FunVal to_expr() tests") {
    CHECK((NEW(FunVal)(NEW(VarExpr)("y"), NEW(VarExpr)("y")))->to_expr()
            ->equals(NEW(FunExpr)(NEW(VarExpr)("y"), NEW(VarExpr)("y"))));
    CHECK((NEW(FunVal)(NEW(VarExpr)("y"), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(1))))->to_expr()
                  ->equals(NEW(FunExpr)(NEW(VarExpr)("y"), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(1)))));
}

TEST_CASE("FunVal add_to() tests") {
    CHECK_THROWS_WITH((NEW(FunVal)(NEW(VarExpr)("y"), NEW(VarExpr)("y")))->add_to(NEW(NumVal)(1)), "Cannot add_to() with a FunVal");
}

TEST_CASE("FunVal multiply_by() tests") {
    CHECK_THROWS_WITH((NEW(FunVal)(NEW(VarExpr)("y"), NEW(VarExpr)("y")))->multiply_by(NEW(NumVal)(1)), "Cannot multiply_by() with a FunVal");
}

TEST_CASE("FunVal is_true() tests") {
    CHECK_THROWS_WITH((NEW(FunVal)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))->is_true(), "Cannot use is_true() on a FunVal");
}

TEST_CASE("FunVal call() tests") {
    CHECK((NEW(FunVal)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))->call(NEW(NumVal)(5))->equals(NEW(NumVal)(1)));
    CHECK((NEW(FunVal)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))->call(NEW(NumVal)(5))->equals(NEW(NumVal)(6)));
    CHECK((NEW(FunVal)(NEW(VarExpr)("x"), NEW(BoolExpr)(true)))->call(NEW(NumVal)(5))->equals(NEW(BoolVal)(true)));
    CHECK_THROWS_WITH((NEW(FunVal)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(BoolExpr)(true))))
            ->call(NEW(NumVal)(5))->equals(NEW(BoolVal)(true)), "Cannot use add_to() with a non-number");
}
