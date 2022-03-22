//
// Created by Kelan Albertson on 1/16/22.
//

#include "MultExpr.h"
#include "AddExpr.h"
#include "NumExpr.h"
#include "NumVal.h"
#include "VarExpr.h"
#include "Env.h"
#include "catch.h"
#include <sstream>

MultExpr::MultExpr(PTR(Expr) lhs, PTR(Expr) rhs) {
    this->lhs_ = lhs;
    this->rhs_ = rhs;
}

bool MultExpr::equals(PTR(Expr) other) {
    PTR(MultExpr) m = CAST(MultExpr)(other);
    if (m == nullptr) {
        return false;
    }
    else {
        return (this->lhs_->equals(m->lhs_) && this->rhs_->equals(m->rhs_));
    }
}

PTR(Val) MultExpr::interp(PTR(Env) env) {
    return (this->lhs_->interp(env)->multiply_by(this->rhs_->interp(env)));
}

//bool MultExpr::has_variable() {
//    return (this->lhs_->has_variable() || this->rhs_->has_variable());
//}

//PTR(Expr) MultExpr::subst(std::string variableName, PTR(Expr) replacement) {
//    return NEW(MultExpr)(this->lhs_->subst(variableName, replacement),
//                        this->rhs_->subst(variableName, replacement));
//}

void MultExpr::print(std::ostream &out) {
    out << "(";
    this->lhs_->print(out);
    out << "*";
    this->rhs_->print(out);
    out << ")";
}

void MultExpr::pretty_print_at(std::ostream &out, precedence_t prec, bool keyword_parentheses, std::streampos &last_new_line_pos) {
    if (prec == prec_mult) {
        out << "(";
    }

    this->lhs_->pretty_print_at(out, prec_mult, true, last_new_line_pos);
    out << " * ";
    this->rhs_->pretty_print_at(out, prec_add, keyword_parentheses, last_new_line_pos);

    if (prec == prec_mult) {
        out << ")";
    }
}

/**
 *************************   TESTS   **************************
 **/

TEST_CASE("MultExpr equals() tests") {
    CHECK((NEW(MultExpr)(NEW(NumExpr)(-1), NEW(NumExpr)(0)))->equals(NEW(MultExpr)(NEW(NumExpr)(-1), NEW(NumExpr)(0))) == true);
    CHECK((NEW(MultExpr)(NEW(NumExpr)(-1), NEW(NumExpr)(0)))->equals(NEW(MultExpr)(NEW(NumExpr)(0), NEW(NumExpr)(-1))) == false);
    CHECK((NEW(MultExpr)(NEW(NumExpr)(-1), NEW(NumExpr)(0)))->equals(NEW(MultExpr)(NEW(NumExpr)(100), NEW(NumExpr)(0))) == false);
    CHECK((NEW(MultExpr)(NEW(NumExpr)(-1), NEW(NumExpr)(0)))->equals(NEW(AddExpr)(NEW(NumExpr)(-1), NEW(NumExpr)(0))) == false);
}

TEST_CASE("MultExpr interp() tests") {
    CHECK((NEW(MultExpr)(NEW(NumExpr)(0), NEW(NumExpr)(0)))->interp(Env::empty)->equals(NEW(NumVal)(0)));
    CHECK((NEW(MultExpr)(NEW(NumExpr)(0), NEW(NumExpr)(1)))->interp(Env::empty)->equals(NEW(NumVal)(0)));
    CHECK((NEW(MultExpr)(NEW(NumExpr)(-1), NEW(NumExpr)(10)))->interp(Env::empty)->equals(NEW(NumVal)(-10)));
    CHECK((NEW(MultExpr)(NEW(NumExpr)(-5), NEW(NumExpr)(-5)))->interp(Env::empty)->equals(NEW(NumVal)(25)));
}

//TEST_CASE("MultExpr has_variable() tests") {
//    CHECK((new MultExpr(new NumExpr(0), new NumExpr(1)))->has_variable() == false);
//    CHECK((new MultExpr(new VarExpr("test"), new NumExpr(1)))->has_variable() == true);
//    CHECK((new MultExpr(new VarExpr("test"), new VarExpr("test")))->has_variable() == true);
//}

//TEST_CASE("MultExpr subst() tests") {
//    CHECK((NEW(MultExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("x")))->subst("x", NEW(VarExpr)("y"))
//            ->equals(NEW(MultExpr)(NEW(VarExpr)("y"), NEW(VarExpr)("y"))));
//    CHECK((NEW(MultExpr)(NEW(VarExpr)("a"), NEW(VarExpr)("x")))->subst("x", NEW(VarExpr)("y"))
//            ->equals(NEW(MultExpr)(NEW(VarExpr)("a"), NEW(VarExpr)("y"))));
//}

TEST_CASE("MultExpr print() tests") {
    CHECK((NEW(MultExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)))->to_string() == "(1*2)");
    CHECK((NEW(MultExpr)(NEW(AddExpr)(NEW(VarExpr)("test"), NEW(NumExpr)(0)), NEW(NumExpr)(1)))->to_string() == "((test+0)*1)");
}

TEST_CASE("MultExpr pretty_print() tests") {
    std::stringstream out("");
    (NEW(MultExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)))->pretty_print(out);
    CHECK(out.str() == "1 * 2");
    out.str(std::string());
    (NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3)), NEW(NumExpr)(4)))->pretty_print(out);
    CHECK(out.str() == "(2 * 3) * 4");
    out.str(std::string());
    (NEW(MultExpr)(NEW(NumExpr)(2), NEW(MultExpr)(NEW(NumExpr)(3), NEW(NumExpr)(4))))->pretty_print(out);
    CHECK(out.str() == "2 * 3 * 4");
    out.str(std::string());
    (NEW(MultExpr)(NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(AddExpr)(NEW(NumExpr)(3), NEW(NumExpr)(4))))
            ->pretty_print(out);
    CHECK(out.str() == "(1 + 2) * (3 + 4)");
}