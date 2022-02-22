//
// Created by Kelan Albertson on 2/20/22.
//

#include "EqExpr.h"
#include "BoolVal.h"
#include "catch.h"
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

bool EqExpr::has_variable() {
    return (this->lhs_->has_variable() || this->rhs_->has_variable());
}

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

void EqExpr::pretty_print_at(std::ostream &out, Expr::precedence_t prec, bool let_parentheses, std::streampos &last_new_line_pos) {
    //TODO
}

/**
 *************************   TESTS   **************************
 **/

TEST_CASE("EqExpr equals() tests") {
}

TEST_CASE("EqExpr interp() tests") {
}

TEST_CASE("EqExpr has_variable() tests") {
}

TEST_CASE("EqExpr subst() tests") {
}

TEST_CASE("EqExpr print()/to_string() tests") {
}

TEST_CASE("EqExpr pretty_print() tests") {
}