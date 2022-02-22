//
// Created by Kelan Albertson on 2/20/22.
//

#include "BoolExpr.h"
#include "BoolVal.h"
#include "NumExpr.h"
#include "catch.h"
#include <sstream>

BoolExpr::BoolExpr(int rep) {
    this->rep_ = rep;
}

bool BoolExpr::equals(Expr *other) {
    BoolExpr* b = dynamic_cast<BoolExpr*>(other);
    if (b == nullptr) {
        return false;
    }
    else {
        return (this->rep_ == b->rep_);
    }
}

Val *BoolExpr::interp() {
    return new BoolVal(this->rep_);
}

bool BoolExpr::has_variable() {
    return false;
}

Expr *BoolExpr::subst(std::string variableName, Expr *replacement) {
    return this;
}

void BoolExpr::print(std::ostream &out) {
    if(this->rep_) {
        out << "_true";
    }
    else {
        out << "_false";
    }
}

void BoolExpr::pretty_print_at(std::ostream &out, Expr::precedence_t prec, bool let_parentheses, std::streampos &last_new_line_pos) {
    //TODO
}

/**
 *************************   TESTS   **************************
 **/

TEST_CASE("BoolExpr equals() tests") {
    CHECK((new BoolExpr(true))->equals(new BoolExpr(true)) == true);
    CHECK((new BoolExpr(false))->equals(new BoolExpr(true)) == false);
    CHECK((new BoolExpr(false))->equals(new NumExpr(0)) == false);
}

TEST_CASE("BoolExpr interp() tests") {
    CHECK((new BoolExpr(true))->interp()->equals(new BoolVal(true)));
    CHECK((new BoolExpr(false))->interp()->equals(new BoolVal(false)));
}

TEST_CASE("BoolExpr has_variable() tests") {
    CHECK((new BoolExpr(false))->has_variable() == false);
}

TEST_CASE("BoolExpr subst() tests") {
    CHECK((new BoolExpr(true))->subst("x", new NumExpr(5))->equals(new BoolExpr(true)));
}

TEST_CASE("BoolExpr print()/to_string() tests") {
    CHECK((new BoolExpr(true))->to_string() == "_true");
    CHECK((new BoolExpr(false))->to_string() == "_false");
}

TEST_CASE("BoolExpr pretty_print() tests") {
}
