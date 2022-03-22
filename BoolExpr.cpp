//
// Created by Kelan Albertson on 2/20/22.
//

#include "BoolExpr.h"
#include "BoolVal.h"
#include "NumExpr.h"
#include "Env.h"
#include "catch.h"
#include <sstream>

BoolExpr::BoolExpr(int rep) {
    this->rep_ = rep;
}

bool BoolExpr::equals(PTR(Expr)other) {
    PTR(BoolExpr) b = CAST(BoolExpr)(other);
    if (b == nullptr) {
        return false;
    }
    else {
        return (this->rep_ == b->rep_);
    }
}

PTR(Val)BoolExpr::interp(PTR(Env) env) {
    return NEW(BoolVal)(this->rep_);
}

//bool BoolExpr::has_variable() {
//    return false;
//}

//PTR(Expr)BoolExpr::subst(std::string variableName, PTR(Expr)replacement) {
//    return THIS;
//}

void BoolExpr::print(std::ostream &out) {
    if(this->rep_) {
        out << "_true";
    }
    else {
        out << "_false";
    }
}

void BoolExpr::pretty_print_at(std::ostream &out, Expr::precedence_t prec, bool keyword_parentheses, std::streampos &last_new_line_pos) {
    this->print(out); //does the exact same thing as print in this case so just reuse it
}

/**
 *************************   TESTS   **************************
 **/

TEST_CASE("BoolExpr equals() tests") {
    CHECK((NEW(BoolExpr)(true))->equals(NEW(BoolExpr)(true)) == true);
    CHECK((NEW(BoolExpr)(false))->equals(NEW(BoolExpr)(true)) == false);
    CHECK((NEW(BoolExpr)(false))->equals(NEW(NumExpr)(0)) == false);
}

TEST_CASE("BoolExpr interp() tests") {
    CHECK((NEW(BoolExpr)(true))->interp(Env::empty)->equals(NEW(BoolVal)(true)));
    CHECK((NEW(BoolExpr)(false))->interp(Env::empty)->equals(NEW(BoolVal)(false)));
}

//TEST_CASE("BoolExpr has_variable() tests") {
//    CHECK((new BoolExpr(false))->has_variable() == false);
//}

//TEST_CASE("BoolExpr subst() tests") {
//    CHECK((NEW(BoolExpr)(true))->subst("x", NEW(NumExpr)(5))->equals(NEW(BoolExpr)(true)));
//}

TEST_CASE("BoolExpr print()/to_string() tests") {
    CHECK((NEW(BoolExpr)(true))->to_string() == "_true");
    CHECK((NEW(BoolExpr)(false))->to_string() == "_false");
}

TEST_CASE("BoolExpr pretty_print() tests") {
    CHECK((NEW(BoolExpr)(true))->to_pretty_string() == "_true");
    CHECK((NEW(BoolExpr)(false))->to_pretty_string() == "_false");
}
