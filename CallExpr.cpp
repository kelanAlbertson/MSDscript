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
#include "Env.h"
#include "ExtendedEnv.h"

CallExpr::CallExpr(PTR(Expr) to_be_called, PTR(Expr) arg) {
    this->to_be_called_ = to_be_called;
    this->arg_ = arg;
}

bool CallExpr::equals(PTR(Expr) other) {
    PTR(CallExpr) c = CAST(CallExpr)(other);
    if (c == nullptr) {
        return false;
    }
    else {
        return (this->to_be_called_->equals(c->to_be_called_) && this->arg_->equals(c->arg_));
    }
}

PTR(Val)CallExpr::interp(PTR(Env) env) {
    return to_be_called_->interp(env)->call(arg_->interp(env));
}

//bool CallExpr::has_variable() {
//    return false;
//}

//PTR(Expr)CallExpr::subst(std::string variableName, PTR(Expr)replacement) {
//    return NEW(CallExpr)(this->to_be_called_->subst(variableName, replacement),
//                        this->arg_->subst(variableName, replacement));
//}

void CallExpr::print(std::ostream &out) {
    this->to_be_called_->print(out);
    out << "(";
    this->arg_->print(out);
    out << ")";
}

void CallExpr::pretty_print_at(std::ostream &out, Expr::precedence_t prec, bool keyword_parentheses, std::streampos &last_new_line_pos) {
    this->to_be_called_->pretty_print_at(out, prec_none, true, last_new_line_pos);

    out << "(";
    this->arg_->pretty_print_at(out, prec_none, false, last_new_line_pos);
    out << ")";
}

/**
 *************************   TESTS   **************************
 **/

TEST_CASE("CallExpr equals() tests") {
    CHECK((NEW(CallExpr)(NEW(FunExpr)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))), NEW(NumExpr)(5)))
            ->equals(NEW(CallExpr)(NEW(FunExpr)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))), NEW(NumExpr)(5))) == true);
    CHECK((NEW(CallExpr)(NEW(FunExpr)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))), NEW(NumExpr)(5)))
            ->equals(NEW(CallExpr)(NEW(FunExpr)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))), NEW(NumExpr)(0))) == false);
    CHECK((NEW(CallExpr)(NEW(FunExpr)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))), NEW(NumExpr)(5)))
            ->equals(NEW(FunExpr)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))) == false);
}

TEST_CASE("CallExpr interp() tests") {
    CHECK((NEW(CallExpr)(NEW(FunExpr)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))), NEW(NumExpr)(5)))
            ->interp(Env::empty)->equals(NEW(NumVal)(6)));
    CHECK((NEW(CallExpr)(NEW(FunExpr)(NEW(VarExpr)("x"), NEW(MultExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("x"))), NEW(NumExpr)(3)))
            ->interp(Env::empty)->equals(NEW(NumVal)(9)));
    CHECK((NEW(CallExpr)(NEW(FunExpr)(NEW(VarExpr)("x"),
                                    NEW(FunExpr)(NEW(VarExpr)("y"),
                                                NEW(AddExpr)(NEW(MultExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("x")), NEW(MultExpr)(NEW(VarExpr)("y"), NEW(VarExpr)("y"))))), NEW(NumExpr)(2)))
                                    ->interp(Env::empty)
                                    ->equals(NEW(FunVal)(NEW(VarExpr)("y"),
                                                        NEW(AddExpr)(NEW(MultExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("x")), NEW(MultExpr)(NEW(VarExpr)("y"), NEW(VarExpr)("y"))), NEW(ExtendedEnv)("x", NEW(NumVal)(2), Env::empty))));
    CHECK((NEW(CallExpr)(NEW(FunExpr)(NEW(VarExpr)("x"),
                                    NEW(CallExpr)(NEW(FunExpr)(NEW(VarExpr)("y"),
                                                             NEW(AddExpr)(NEW(MultExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("x")), NEW(MultExpr)(NEW(VarExpr)("y"), NEW(VarExpr)("y")))),
                                                 NEW(NumExpr)(3))),
                        NEW(NumExpr)(2)))->interp(Env::empty)->equals(NEW(NumVal)(13)));
    CHECK((NEW(CallExpr)(NEW(FunExpr)(NEW(VarExpr)("b"), NEW(BoolExpr)(true)), NEW(NumExpr)(-99)))->interp(Env::empty)->equals(NEW(BoolVal)(true)));
    CHECK_THROWS_WITH((NEW(CallExpr)(NEW(BoolExpr)(false), NEW(NumExpr)(1)))->interp(Env::empty), "Cannot call() a BoolVal");
}

//TEST_CASE("CallExpr subst() tests") {
//    CHECK((NEW(CallExpr)(NEW(FunExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("x")), NEW(NumExpr)(1)))
//            ->subst("x", NEW(NumExpr)(0))->equals(NEW(CallExpr)(NEW(FunExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("x")), NEW(NumExpr)(1))));
//    CHECK((NEW(CallExpr)(NEW(FunExpr)(NEW(VarExpr)("x"), NEW(MultExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y"))), NEW(NumExpr)(2)))
//                  ->subst("y", NEW(NumExpr)(8))->equals(NEW(CallExpr)(NEW(FunExpr)(NEW(VarExpr)("x"), NEW(MultExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(8))), NEW(NumExpr)(2))));
//    CHECK((NEW(CallExpr)(NEW(FunExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("x")), NEW(VarExpr)("x")))
//                  ->subst("x", NEW(NumExpr)(0))->equals(NEW(CallExpr)(NEW(FunExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("x")), NEW(NumExpr)(0))));
//}

TEST_CASE("CallExpr print() tests") {
    CHECK((NEW(CallExpr)(NEW(FunExpr)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))), NEW(NumExpr)(1)))
            ->to_string() == "(_fun (x) (x+1))(1)");
    CHECK((NEW(CallExpr)(NEW(FunExpr)(NEW(VarExpr)("y"), NEW(BoolExpr)(false)), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))
            ->to_string() == "(_fun (y) _false)((x+1))");
    CHECK((NEW(CallExpr)(NEW(FunExpr)(NEW(VarExpr)("x"), NEW(CallExpr)(NEW(FunExpr)(NEW(VarExpr)("y"), NEW(AddExpr)(NEW(MultExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("x")), NEW(MultExpr)(NEW(VarExpr)("y"), NEW(VarExpr)("y")))), NEW(NumExpr)(3))),NEW(NumExpr)(2)))
            ->to_string() == "(_fun (x) (_fun (y) ((x*x)+(y*y)))(3))(2)");
}

TEST_CASE("CallExpr pretty_print() tests") {
    CHECK((NEW(CallExpr)(NEW(FunExpr)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))), NEW(NumExpr)(1)))
        ->to_pretty_string() == "(_fun (x)\n"
                                "   x + 1)(1)");
    CHECK((NEW(CallExpr)(NEW(BoolExpr)(false), NEW(NumExpr)(1)))->to_pretty_string() == "_false(1)");
    CHECK((NEW(CallExpr)(NEW(FunExpr)(NEW(VarExpr)("x"), NEW(CallExpr)(NEW(FunExpr)(NEW(VarExpr)("y"), NEW(AddExpr)(NEW(MultExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("x")), NEW(MultExpr)(NEW(VarExpr)("y"), NEW(VarExpr)("y")))), NEW(NumExpr)(3))),NEW(NumExpr)(2)))
            ->to_pretty_string() == "(_fun (x)\n"
                                    "   (_fun (y)\n"
                                    "      x * x + y * y)(3))(2)");
}