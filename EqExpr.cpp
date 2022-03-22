//
// Created by Kelan Albertson on 2/20/22.
//

#include "EqExpr.h"
#include "BoolVal.h"
#include "catch.h"
#include "NumExpr.h"
#include "VarExpr.h"
#include "BoolExpr.h"
#include "AddExpr.h"
#include "MultExpr.h"
#include "Env.h"
#include <sstream>

EqExpr::EqExpr(PTR(Expr)lhs, PTR(Expr)rhs) {
    this->lhs_ = lhs;
    this->rhs_ = rhs;
}

bool EqExpr::equals(PTR(Expr) other) {
    PTR(EqExpr) e = CAST(EqExpr)(other);
    if (e == nullptr) {
        return false;
    }
    else {
        return (this->lhs_->equals(e->lhs_) && this->rhs_->equals(e->rhs_));
    }
}

PTR(Val) EqExpr::interp(PTR(Env) env) {
    return NEW(BoolVal)(this->lhs_->interp(env)->equals(this->rhs_->interp(env)));
}

//bool EqExpr::has_variable() {
//    return (this->lhs_->has_variable() || this->rhs_->has_variable());
//}

//PTR(Expr) EqExpr::subst(std::string variableName, PTR(Expr) replacement) {
//    return NEW(EqExpr)(this->lhs_->subst(variableName, replacement),
//                      this->rhs_->subst(variableName, replacement));
//}

void EqExpr::print(std::ostream &out) {
    out << "(";
    this->lhs_->print(out);
    out << "==";
    this->rhs_->print(out);
    out << ")";
}

void EqExpr::pretty_print_at(std::ostream &out, Expr::precedence_t prec, bool keyword_parentheses, std::streampos &last_new_line_pos) {
    if (prec >= prec_eq) {
        out << "(";
    }

    this->lhs_->pretty_print_at(out, prec_eq, true, last_new_line_pos);
    out << " == ";
    this->rhs_->pretty_print_at(out, prec_none, false, last_new_line_pos);

    if (prec >= prec_eq) {
        out << ")";
    }
}

/**
 *************************   TESTS   **************************
 **/

TEST_CASE("EqExpr equals() tests") {
    CHECK((NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(1)))->equals(NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(1))) == true);
    CHECK((NEW(EqExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))->equals(NEW(EqExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))) == true);
    CHECK((NEW(EqExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))->equals(NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(1))) == false);
    CHECK((NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(1)))->equals(NEW(BoolExpr)(true)) == false);
}

TEST_CASE("EqExpr interp() tests") {
    CHECK((NEW(EqExpr)(NEW(NumExpr)(0), NEW(NumExpr)(0)))->interp(Env::empty)->equals(NEW(BoolVal)(true)));
    CHECK((NEW(EqExpr)(NEW(NumExpr)(-99), NEW(NumExpr)(0)))->interp(Env::empty)->equals(NEW(BoolVal)(false)));
    CHECK((NEW(EqExpr)(NEW(BoolExpr)(true), NEW(BoolExpr)(true)))->interp(Env::empty)->equals(NEW(BoolVal)(true)));
    CHECK((NEW(EqExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(1)))->interp(Env::empty)->equals(NEW(BoolVal)(false)));
    CHECK_THROWS_WITH((NEW(EqExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2)))->interp(Env::empty)->equals(NEW(BoolVal)(false)), "free variable: x");
}

//TEST_CASE("EqExpr has_variable() tests") {
//    CHECK((new EqExpr(new NumExpr(1), new NumExpr(1)))->has_variable() == false);
//    CHECK((new EqExpr(new VarExpr("y"), new NumExpr(1)))->has_variable() == true);
//    CHECK((new EqExpr(new NumExpr(1), new VarExpr("test")))->has_variable() == true);
//    CHECK((new EqExpr(new NumExpr(1), new AddExpr(new NumExpr(1), new VarExpr("x"))))->has_variable() == true);
//}

//TEST_CASE("EqExpr subst() tests") {
//    CHECK((NEW(EqExpr)(NEW(VarExpr)("y"), NEW(VarExpr)("y")))->subst("y", NEW(NumExpr)(20))
//            ->equals(NEW(EqExpr)(NEW(NumExpr)(20), NEW(NumExpr)(20))));
//    CHECK((NEW(EqExpr)(NEW(NumExpr)(0), NEW(NumExpr)(0)))->subst("x", NEW(NumExpr)(-1))
//            ->equals(NEW(EqExpr)(NEW(NumExpr)(0), NEW(NumExpr)(0))));
//    CHECK((NEW(EqExpr)(NEW(NumExpr)(1), NEW(VarExpr)("test")))->subst("t", NEW(NumExpr)(0))
//            ->equals(NEW(EqExpr)(NEW(NumExpr)(1), NEW(VarExpr)("test"))));
//    CHECK((NEW(EqExpr)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(NumExpr)(3), NEW(VarExpr)("y"))))->subst("y", NEW(NumExpr)(0))
//                  ->equals(NEW(EqExpr)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(NumExpr)(3), NEW(NumExpr)(0)))));
//}

TEST_CASE("EqExpr print()/to_string() tests") {
    CHECK((NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(1)))->to_string() == "(1==1)");
    CHECK((NEW(EqExpr)(NEW(VarExpr)("zero"), NEW(NumExpr)(0)))->to_string() == "(zero==0)");
    CHECK((NEW(EqExpr)(NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(1)), NEW(VarExpr)("y")))->to_string() == "((1+1)==y)");
    CHECK((NEW(EqExpr)(NEW(BoolExpr)(false), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))->to_string() == "(_false==(x+1))");
}

TEST_CASE("EqExpr pretty_print() tests") {
    CHECK((NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(1)))->to_pretty_string() == "1 == 1");
    CHECK((NEW(EqExpr)(NEW(VarExpr)("zero"), NEW(NumExpr)(0)))->to_pretty_string() == "zero == 0");
    CHECK((NEW(EqExpr)(NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(1)), NEW(VarExpr)("y")))
            ->to_pretty_string() == "1 + 1 == y");
    CHECK((NEW(AddExpr)(NEW(BoolExpr)(false), NEW(EqExpr)(NEW(NumExpr)(0), NEW(BoolExpr)(false))))
            ->to_pretty_string() == "_false + (0 == _false)");
    CHECK((NEW(EqExpr)(NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(NumExpr)(3)))
            ->to_pretty_string() == "(1 == 2) == 3");
    CHECK((NEW(AddExpr)(NEW(MultExpr)(NEW(EqExpr)(NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)), NEW(EqExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(9))), NEW(VarExpr)("x")), NEW(NumExpr)(1)))
            ->to_pretty_string() == "(x + 1 == _true == 9) * x + 1");
}