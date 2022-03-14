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
#include <sstream>

EqExpr::EqExpr(Expr *lhs, Expr *rhs) {
    this->lhs_ = lhs;
    this->rhs_ = rhs;
}

bool EqExpr::equals(Expr *other) {
    EqExpr* e = dynamic_cast<EqExpr*>(other);
    if (e == nullptr) {
        return false;
    }
    else {
        return (this->lhs_->equals(e->lhs_) && this->rhs_->equals(e->rhs_));
    }
}

Val *EqExpr::interp() {
    return new BoolVal(this->lhs_->interp()->equals(this->rhs_->interp()));
}

//bool EqExpr::has_variable() {
//    return (this->lhs_->has_variable() || this->rhs_->has_variable());
//}

Expr *EqExpr::subst(std::string variableName, Expr *replacement) {
    return new EqExpr(this->lhs_->subst(variableName, replacement),
                      this->rhs_->subst(variableName, replacement));
}

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
    CHECK((new EqExpr(new NumExpr(1), new NumExpr(1)))->equals(new EqExpr(new NumExpr(1), new NumExpr(1))) == true);
    CHECK((new EqExpr(new VarExpr("x"), new NumExpr(1)))->equals(new EqExpr(new VarExpr("x"), new NumExpr(1))) == true);
    CHECK((new EqExpr(new VarExpr("x"), new NumExpr(1)))->equals(new EqExpr(new NumExpr(1), new NumExpr(1))) == false);
    CHECK((new EqExpr(new NumExpr(1), new NumExpr(1)))->equals(new BoolExpr(true)) == false);
}

TEST_CASE("EqExpr interp() tests") {
    CHECK((new EqExpr(new NumExpr(0), new NumExpr(0)))->interp()->equals(new BoolVal(true)));
    CHECK((new EqExpr(new NumExpr(-99), new NumExpr(0)))->interp()->equals(new BoolVal(false)));
    CHECK((new EqExpr(new BoolExpr(true), new BoolExpr(true)))->interp()->equals(new BoolVal(true)));
    CHECK((new EqExpr(new BoolExpr(true), new NumExpr(1)))->interp()->equals(new BoolVal(false)));
    CHECK_THROWS_WITH((new EqExpr(new VarExpr("x"), new NumExpr(2)))->interp()->equals(new BoolVal(false)), "VarExpr cannot be interpreted");
}

//TEST_CASE("EqExpr has_variable() tests") {
//    CHECK((new EqExpr(new NumExpr(1), new NumExpr(1)))->has_variable() == false);
//    CHECK((new EqExpr(new VarExpr("y"), new NumExpr(1)))->has_variable() == true);
//    CHECK((new EqExpr(new NumExpr(1), new VarExpr("test")))->has_variable() == true);
//    CHECK((new EqExpr(new NumExpr(1), new AddExpr(new NumExpr(1), new VarExpr("x"))))->has_variable() == true);
//}

TEST_CASE("EqExpr subst() tests") {
    CHECK((new EqExpr(new VarExpr("y"), new VarExpr("y")))->subst("y", new NumExpr(20))
            ->equals(new EqExpr(new NumExpr(20), new NumExpr(20))));
    CHECK((new EqExpr(new NumExpr(0), new NumExpr(0)))->subst("x", new NumExpr(-1))
            ->equals(new EqExpr(new NumExpr(0), new NumExpr(0))));
    CHECK((new EqExpr(new NumExpr(1), new VarExpr("test")))->subst("t", new NumExpr(0))
            ->equals(new EqExpr(new NumExpr(1), new VarExpr("test"))));
    CHECK((new EqExpr(new VarExpr("x"), new AddExpr(new NumExpr(3), new VarExpr("y"))))->subst("y", new NumExpr(0))
                  ->equals(new EqExpr(new VarExpr("x"), new AddExpr(new NumExpr(3), new NumExpr(0)))));
}

TEST_CASE("EqExpr print()/to_string() tests") {
    CHECK((new EqExpr(new NumExpr(1), new NumExpr(1)))->to_string() == "(1==1)");
    CHECK((new EqExpr(new VarExpr("zero"), new NumExpr(0)))->to_string() == "(zero==0)");
    CHECK((new EqExpr(new AddExpr(new NumExpr(1), new NumExpr(1)), new VarExpr("y")))->to_string() == "((1+1)==y)");
    CHECK((new EqExpr(new BoolExpr(false), new AddExpr(new VarExpr("x"), new NumExpr(1))))->to_string() == "(_false==(x+1))");
}

TEST_CASE("EqExpr pretty_print() tests") {
    CHECK((new EqExpr(new NumExpr(1), new NumExpr(1)))->to_pretty_string() == "1 == 1");
    CHECK((new EqExpr(new VarExpr("zero"), new NumExpr(0)))->to_pretty_string() == "zero == 0");
    CHECK((new EqExpr(new AddExpr(new NumExpr(1), new NumExpr(1)), new VarExpr("y")))
            ->to_pretty_string() == "1 + 1 == y");
    CHECK((new AddExpr(new BoolExpr(false), new EqExpr(new NumExpr(0), new BoolExpr(false))))
            ->to_pretty_string() == "_false + (0 == _false)");
    CHECK((new EqExpr(new EqExpr(new NumExpr(1), new NumExpr(2)), new NumExpr(3)))
            ->to_pretty_string() == "(1 == 2) == 3");
    CHECK((new AddExpr(new MultExpr(new EqExpr(new AddExpr(new VarExpr("x"), new NumExpr(1)), new EqExpr(new BoolExpr(true), new NumExpr(9))), new VarExpr("x")), new NumExpr(1)))
            ->to_pretty_string() == "(x + 1 == _true == 9) * x + 1");
}