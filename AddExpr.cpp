//
// Created by Kelan Albertson on 1/16/22.
//

#include "AddExpr.h"
#include "MultExpr.h"
#include "NumExpr.h"
#include "NumVal.h"
#include "VarExpr.h"
#include "catch.h"
#include <sstream>

AddExpr::AddExpr(PTR(Expr) lhs, PTR(Expr) rhs) {
    this->lhs_ = lhs;
    this->rhs_ = rhs;
}

bool AddExpr::equals(PTR(Expr) other) {
    PTR(AddExpr) a = CAST(AddExpr)(other);
    if (a == nullptr) {
        return false;
    }
    else {
        return (this->lhs_->equals(a->lhs_) && this->rhs_->equals(a->rhs_));
    }
}

PTR(Val) AddExpr::interp(PTR(Env) env) {
    return (this->lhs_->interp(env)->add_to(this->rhs_->interp(env)));
}

//bool AddExpr::has_variable() {
//    return (this->lhs_->has_variable() || this->rhs_->has_variable());
//}

//PTR(Expr) AddExpr::subst(std::string variableName, PTR(Expr) replacement) {
//    return NEW(AddExpr)(this->lhs_->subst(variableName, replacement),
//                       this->rhs_->subst(variableName, replacement));
//}

void AddExpr::print(std::ostream &out) {
    out << "(";
    this->lhs_->print(out);
    out << "+";
    this->rhs_->print(out);
    out << ")";
}

void AddExpr::pretty_print_at(std::ostream &out, precedence_t prec, bool keyword_parentheses, std::streampos &last_new_line_pos) {
    if (prec >= prec_add) {
        out << "(";
    }

    this->lhs_->pretty_print_at(out, prec_add, true, last_new_line_pos);
    out << " + ";
    this->rhs_->pretty_print_at(out, prec_eq, false, last_new_line_pos);

    if (prec >= prec_add) {
        out << ")";
    }
}

/**
 *************************   TESTS   **************************
 **/

TEST_CASE("AddExpr equals() tests") {
    CHECK((NEW(AddExpr)(NEW(NumExpr)(0), NEW(NumExpr)(1)))->equals(NEW(AddExpr)(NEW(NumExpr)(0), NEW(NumExpr)(1))) == true);
    CHECK((NEW(AddExpr)(NEW(NumExpr)(0), NEW(NumExpr)(1)))->equals(NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(0))) == false);
    CHECK((NEW(AddExpr)(NEW(NumExpr)(0), NEW(NumExpr)(1)))->equals(NEW(AddExpr)(NEW(NumExpr)(-20), NEW(NumExpr)(9))) == false);
    CHECK((NEW(AddExpr)(NEW(NumExpr)(0), NEW(NumExpr)(1)))->equals(NEW(NumExpr)(1)) == false);
}

//TODO
TEST_CASE("AddExpr interp() tests") {
    CHECK((NEW(AddExpr)(NEW(NumExpr)(0), NEW(NumExpr)(0)))->interp()->equals(NEW(NumVal)(0)));
    CHECK((NEW(AddExpr)(NEW(NumExpr)(0), NEW(NumExpr)(1)))->interp()->equals(NEW(NumVal)(1)));
    CHECK((NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(0)))->interp()->equals(NEW(NumVal)(1)));
    CHECK((NEW(AddExpr)(NEW(NumExpr)(-5), NEW(NumExpr)(18)))->interp()->equals(NEW(NumVal)(13)));
}

//TEST_CASE("AddExpr has_variable() tests") {
//    CHECK((new AddExpr(new NumExpr(0), new NumExpr(1)))->has_variable() == false);
//    CHECK((new AddExpr(new VarExpr("test"), new NumExpr(1)))->has_variable() == true);
//    CHECK((new AddExpr(new VarExpr("test"), new VarExpr("test")))->has_variable() == true);
//}

//TEST_CASE("AddExpr subst() tests") {
//    CHECK((NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(7)))->subst("x", NEW(VarExpr)("y"))
//            ->equals(NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(7))));
//    CHECK((NEW(AddExpr)(NEW(VarExpr)("a"), NEW(NumExpr)(7)))->subst("x", NEW(VarExpr)("y"))
//            ->equals(NEW(AddExpr)(NEW(VarExpr)("a"), NEW(NumExpr)(7))));
//
//    CHECK((NEW(AddExpr)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(-1), NEW(VarExpr)("x")), NEW(VarExpr)("a"))))->subst("x", NEW(VarExpr)("y"))
//            -> equals(NEW(AddExpr)(NEW(VarExpr)("y"), NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(-1), NEW(VarExpr)("y")), NEW(VarExpr)("a")))));
//}

TEST_CASE("AddExpr print()/to_string() tests") {
    CHECK((NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)))->to_string() == "(1+2)");
    CHECK((NEW(AddExpr)(NEW(NumExpr)(1), NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3))))->to_string() == "(1+(2+3))");
    CHECK((NEW(AddExpr)(NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(NumExpr)(3)))->to_string() == "((1+2)+3)");
}

TEST_CASE("AddExpr pretty_print() tests") {
    std::stringstream out("");
    (NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)))->pretty_print(out);
    CHECK(out.str() == "1 + 2");
    out.str(std::string());
    (NEW(AddExpr)(NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(NumExpr)(3)))->pretty_print(out);
    CHECK(out.str() == "(1 + 2) + 3");
    out.str(std::string());
    (NEW(AddExpr)(NEW(NumExpr)(1), NEW(MultExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3))))->pretty_print(out);
    CHECK(out.str() == "1 + 2 * 3");
}