//
// Created by Kelan Albertson on 2/26/22.
//

#include "CallExpr.h"
#include "Val.h"
#include "catch.h"
#include "FunExpr.h"
#include "AddExpr.h"
#include "NumExpr.h"
#include "VarExpr.h"
#include "NumVal.h"
#include "MultExpr.h"
#include "FunVal.h"
#include "BoolExpr.h"
#include "BoolVal.h"

CallExpr::CallExpr(Expr *to_be_called, Expr *arg) {
    this->to_be_called_ = to_be_called;
    this->arg_ = arg;
}

bool CallExpr::equals(Expr *other) {
    CallExpr *c = dynamic_cast<CallExpr*>(other);
    if (c == nullptr) {
        return false;
    }
    else {
        return (this->to_be_called_->equals(c->to_be_called_) && this->arg_->equals(c->arg_));
    }
}

Val *CallExpr::interp() {
    return to_be_called_->interp()->call(arg_->interp());
}

bool CallExpr::has_variable() {
    return false;
    //TODO
}

Expr *CallExpr::subst(std::string variableName, Expr *replacement) {
    return new CallExpr(this->to_be_called_->subst(variableName, replacement),
                        this->arg_->subst(variableName, replacement));
}

void CallExpr::print(std::ostream &out) {
    this->to_be_called_->print(out);
    out << "(";
    this->arg_->print(out);
    out << ")";
}

void CallExpr::pretty_print_at(std::ostream &out, Expr::precedence_t prec, bool keyword_parentheses, std::streampos &last_new_line_pos) {
    this->print(out);
}

/**
 *************************   TESTS   **************************
 **/

TEST_CASE("CallExpr equals() tests") {
    CHECK((new CallExpr(new FunExpr(new VarExpr("x"), new AddExpr(new VarExpr("x"), new NumExpr(1))), new NumExpr(5)))
            ->equals(new CallExpr(new FunExpr(new VarExpr("x"), new AddExpr(new VarExpr("x"), new NumExpr(1))), new NumExpr(5))) == true);
    CHECK((new CallExpr(new FunExpr(new VarExpr("x"), new AddExpr(new VarExpr("x"), new NumExpr(1))), new NumExpr(5)))
            ->equals(new CallExpr(new FunExpr(new VarExpr("x"), new AddExpr(new VarExpr("x"), new NumExpr(1))), new NumExpr(0))) == false);
    CHECK((new CallExpr(new FunExpr(new VarExpr("x"), new AddExpr(new VarExpr("x"), new NumExpr(1))), new NumExpr(5)))
            ->equals(new FunExpr(new VarExpr("x"), new AddExpr(new VarExpr("x"), new NumExpr(1)))) == false);
}

TEST_CASE("CallExpr interp() tests") {
    CHECK((new CallExpr(new FunExpr(new VarExpr("x"), new AddExpr(new VarExpr("x"), new NumExpr(1))), new NumExpr(5)))
            ->interp()->equals(new NumVal(6)));
    CHECK((new CallExpr(new FunExpr(new VarExpr("x"), new MultExpr(new VarExpr("x"), new VarExpr("x"))), new NumExpr(3)))
            ->interp()->equals(new NumVal(9)));
    CHECK((new CallExpr(new FunExpr(new VarExpr("x"),
                                    new FunExpr(new VarExpr("y"),
                                                new AddExpr(new MultExpr(new VarExpr("x"), new VarExpr("x")), new MultExpr(new VarExpr("y"), new VarExpr("y"))))), new NumExpr(2)))
                                    ->interp()
                                    ->equals(new FunVal(new VarExpr("y"),
                                                        new AddExpr(new MultExpr(new NumExpr(2), new NumExpr(2)), new MultExpr(new VarExpr("y"), new VarExpr("y"))))));
    CHECK((new CallExpr(new FunExpr(new VarExpr("x"),
                                    new CallExpr(new FunExpr(new VarExpr("y"),
                                                             new AddExpr(new MultExpr(new VarExpr("x"), new VarExpr("x")), new MultExpr(new VarExpr("y"), new VarExpr("y")))),
                                                 new NumExpr(3))),
                        new NumExpr(2)))->interp()->equals(new NumVal(13)));
    CHECK((new CallExpr(new FunExpr(new VarExpr("b"), new BoolExpr(true)), new NumExpr(-99)))->interp()->equals(new BoolVal(true)));
    CHECK_THROWS_WITH((new CallExpr(new BoolExpr(false), new NumExpr(1)))->interp(), "Cannot call() a BoolVal");
}

TEST_CASE("CallExpr subst() tests") {
    CHECK((new CallExpr(new FunExpr(new VarExpr("x"), new VarExpr("x")), new NumExpr(1)))
            ->subst("x", new NumExpr(0))->equals(new CallExpr(new FunExpr(new VarExpr("x"), new VarExpr("x")), new NumExpr(1))));
    CHECK((new CallExpr(new FunExpr(new VarExpr("x"), new MultExpr(new VarExpr("x"), new VarExpr("y"))), new NumExpr(2)))
                  ->subst("y", new NumExpr(8))->equals(new CallExpr(new FunExpr(new VarExpr("x"), new MultExpr(new VarExpr("x"), new NumExpr(8))), new NumExpr(2))));
    CHECK((new CallExpr(new FunExpr(new VarExpr("x"), new VarExpr("x")), new VarExpr("x")))
                  ->subst("x", new NumExpr(0))->equals(new CallExpr(new FunExpr(new VarExpr("x"), new VarExpr("x")), new NumExpr(0))));
}

TEST_CASE("CallExpr print() tests") {
    CHECK((new CallExpr(new FunExpr(new VarExpr("x"), new AddExpr(new VarExpr("x"), new NumExpr(1))), new NumExpr(1)))
            ->to_string() == "(_fun (x) (x+1))(1)");
    CHECK((new CallExpr(new FunExpr(new VarExpr("y"), new BoolExpr(false)), new AddExpr(new VarExpr("x"), new NumExpr(1))))
            ->to_string() == "(_fun (y) _false)((x+1))");
    CHECK((new CallExpr(new FunExpr(new VarExpr("x"), new CallExpr(new FunExpr(new VarExpr("y"), new AddExpr(new MultExpr(new VarExpr("x"), new VarExpr("x")), new MultExpr(new VarExpr("y"), new VarExpr("y")))), new NumExpr(3))),new NumExpr(2)))
            ->to_string() == "(_fun (x) (_fun (y) ((x*x)+(y*y)))(3))(2)");
}

TEST_CASE("CallExpr pretty_print() tests") {
    CHECK((new CallExpr(new FunExpr(new VarExpr("x"), new AddExpr(new VarExpr("x"), new NumExpr(1))), new NumExpr(1)))
                  ->to_pretty_string() == "(_fun (x) (x+1))(1)");
}