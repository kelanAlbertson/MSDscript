//
// Created by Kelan Albertson on 2/20/22.
//

#include "IfExpr.h"
#include "Val.h"
#include "AddExpr.h"
#include "NumExpr.h"
#include "MultExpr.h"
#include "VarExpr.h"
#include "catch.h"
#include "EqExpr.h"
#include "BoolExpr.h"
#include "BoolVal.h"
#include "NumVal.h"
#include <sstream>

IfExpr::IfExpr(Expr *condition, Expr *ts, Expr *es) {
    this->condition_ = condition;
    this->then_statement_ = ts;
    this->else_statement_ = es;
}

bool IfExpr::equals(Expr *other) {
    IfExpr* i = dynamic_cast<IfExpr*>(other);
    if (i == nullptr) {
        return false;
    }
    else {
        return (this->condition_->equals(i->condition_)
                && this->then_statement_->equals(i->then_statement_)
                && this->else_statement_->equals(i->else_statement_));
    }
}

Val *IfExpr::interp() {
    if (this->condition_->interp()->is_true()) {
        return this->then_statement_->interp();
    }
    else {
        return this->else_statement_->interp();
    }
}

bool IfExpr::has_variable() {
    return (this->condition_->has_variable() || this->then_statement_->has_variable() || this->else_statement_->has_variable());
}

Expr *IfExpr::subst(std::string variableName, Expr *replacement) {
    return new IfExpr(this->condition_->subst(variableName, replacement),
                      this->then_statement_->subst(variableName, replacement),
                      this->else_statement_->subst(variableName, replacement));
}

void IfExpr::print(std::ostream &out) {
    out << "(_if ";
    this->condition_->print(out);
    out << " _then ";
    this->then_statement_->print(out);
    out << " _else ";
    this->else_statement_->print(out);
    out << ")";
}

void IfExpr::pretty_print_at(std::ostream &out, Expr::precedence_t prec, bool let_parentheses, std::streampos &last_new_line_pos) {
    //TODO
}

/**
 *************************   TESTS   **************************
 **/

TEST_CASE("IfExpr equals() tests") {
    CHECK((new IfExpr(new EqExpr(new NumExpr(1), new NumExpr(2)), new BoolExpr(true), new BoolExpr(false)))
            ->equals(new IfExpr(new EqExpr(new NumExpr(1), new NumExpr(2)), new BoolExpr(true), new BoolExpr(false))) == true);
    CHECK((new IfExpr(new BoolExpr(false), new NumExpr(0), new VarExpr("y")))
                  ->equals(new IfExpr(new BoolExpr(true), new NumExpr(0), new VarExpr("y"))) == false);
    CHECK((new IfExpr(new EqExpr(new VarExpr("x"), new VarExpr("x")), new NumExpr(1), new NumExpr(0)))
                  ->equals(new EqExpr(new VarExpr("x"), new VarExpr("x"))) == false);
}

TEST_CASE("IfExpr interp() tests") {
    CHECK((new IfExpr(new EqExpr(new NumExpr(1), new NumExpr(2)), new BoolExpr(true), new BoolExpr(false)))
            ->interp()->equals(new BoolVal(false)));
    CHECK_THROWS_WITH((new IfExpr(new BoolExpr(false), new NumExpr(0), new VarExpr("y")))->interp(), "VarExpr cannot be interpreted");
    CHECK((new IfExpr(new EqExpr(new AddExpr(new NumExpr(3), new NumExpr(1)), new AddExpr(new NumExpr(2), new NumExpr(2))), new NumExpr(25), new NumExpr(100)))->interp()->equals(new NumVal(25)));
    CHECK((new IfExpr(new BoolExpr(false), new NumExpr(-1), new BoolExpr(false)))->interp()->equals(new BoolVal(false)));
}

TEST_CASE("IfExpr has_variable() tests") {
    CHECK((new IfExpr(new EqExpr(new NumExpr(0), new NumExpr(1)), new VarExpr("x"), new VarExpr("y")))->has_variable() == true);
    CHECK((new IfExpr(new BoolExpr(true), new NumExpr(1), new NumExpr(2)))->has_variable() == false);
}

TEST_CASE("IfExpr subst() tests") {
}

TEST_CASE("IfExpr print()/to_string() tests") {
}

TEST_CASE("IfExpr pretty_print() tests") {
}