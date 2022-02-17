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

AddExpr::AddExpr(Expr* lhs, Expr* rhs) {
    this->lhs_ = lhs;
    this->rhs_ = rhs;
}

bool AddExpr::equals(Expr* other) {
    AddExpr* a = dynamic_cast<AddExpr*>(other);
    if (a == nullptr) {
        return false;
    }
    else {
        return (this->lhs_->equals(a->lhs_) && this->rhs_->equals(a->rhs_));
    }
}

Val * AddExpr::interp() {
    return (this->lhs_->interp()->add_to(this->rhs_->interp()));
}

bool AddExpr::has_variable() {
    return (this->lhs_->has_variable() || this->rhs_->has_variable());
}

Expr* AddExpr::subst(std::string variableName, Expr *replacement) {
    return new AddExpr(this->lhs_->subst(variableName, replacement),
                       this->rhs_->subst(variableName, replacement));
}

void AddExpr::print(std::ostream &out) {
    out << "(";
    this->lhs_->print(out);
    out << "+";
    this->rhs_->print(out);
    out << ")";
}

void AddExpr::pretty_print_at(std::ostream &out, precedence_t prec, bool let_parentheses, std::streampos &last_new_line_pos) {
    if (prec >= prec_add) {
        out << "(";
    }

    this->lhs_->pretty_print_at(out, prec_add, true, last_new_line_pos);
    out << " + ";
    this->rhs_->pretty_print_at(out, prec_none, false, last_new_line_pos);

    if (prec >= prec_add) {
        out << ")";
    }
}

/**
 *************************   TESTS   **************************
 **/

TEST_CASE("AddExpr equals() tests") {
    CHECK((new AddExpr(new NumExpr(0), new NumExpr(1)))->equals(new AddExpr(new NumExpr(0), new NumExpr(1))) == true);
    CHECK((new AddExpr(new NumExpr(0), new NumExpr(1)))->equals(new AddExpr(new NumExpr(1), new NumExpr(0))) == false);
    CHECK((new AddExpr(new NumExpr(0), new NumExpr(1)))->equals(new AddExpr(new NumExpr(-20), new NumExpr(9))) == false);
    CHECK((new AddExpr(new NumExpr(0), new NumExpr(1)))->equals(new NumExpr(1)) == false);
}

TEST_CASE("AddExpr interp() tests") {
    CHECK((new AddExpr(new NumExpr(0), new NumExpr(0)))->interp() == 0);
    CHECK((new AddExpr(new NumExpr(0), new NumExpr(1)))->interp() == 1);
    CHECK((new AddExpr(new NumExpr(1), new NumExpr(0)))->interp() == 1);
    CHECK((new AddExpr(new NumExpr(-5), new NumExpr(18)))->interp() == 13);
}

TEST_CASE("AddExpr has_variable() tests") {
    CHECK((new AddExpr(new NumExpr(0), new NumExpr(1)))->has_variable() == false);
    CHECK((new AddExpr(new VarExpr("test"), new NumExpr(1)))->has_variable() == true);
    CHECK((new AddExpr(new VarExpr("test"), new VarExpr("test")))->has_variable() == true);
}

TEST_CASE("AddExpr subst() tests") {
    CHECK((new AddExpr(new VarExpr("x"), new NumExpr(7)))->subst("x", new VarExpr("y"))
            ->equals(new AddExpr(new VarExpr("y"), new NumExpr(7))));
    CHECK((new AddExpr(new VarExpr("a"), new NumExpr(7)))->subst("x", new VarExpr("y"))
            ->equals(new AddExpr(new VarExpr("a"), new NumExpr(7))));

    CHECK((new AddExpr(new VarExpr("x"), new AddExpr(new MultExpr(new NumExpr(-1), new VarExpr("x")), new VarExpr("a"))))->subst("x", new VarExpr("y"))
            -> equals(new AddExpr(new VarExpr("y"), new AddExpr(new MultExpr(new NumExpr(-1), new VarExpr("y")), new VarExpr("a")))));
}

TEST_CASE("AddExpr print()/to_string() tests") {
    CHECK((new AddExpr(new NumExpr(1), new NumExpr(2)))->to_string() == "(1+2)");
    CHECK((new AddExpr(new NumExpr(1), new AddExpr(new NumExpr(2), new NumExpr(3))))->to_string() == "(1+(2+3))");
    CHECK((new AddExpr(new AddExpr(new NumExpr(1), new NumExpr(2)), new NumExpr(3)))->to_string() == "((1+2)+3)");
}

TEST_CASE("AddExpr pretty_print() tests") {
    std::stringstream out("");
    (new AddExpr(new NumExpr(1), new NumExpr(2)))->pretty_print(out);
    CHECK(out.str() == "1 + 2");
    out.str(std::string());
    (new AddExpr(new AddExpr(new NumExpr(1), new NumExpr(2)), new NumExpr(3)))->pretty_print(out);
    CHECK(out.str() == "(1 + 2) + 3");
    out.str(std::string());
    (new AddExpr(new NumExpr(1), new MultExpr(new NumExpr(2), new NumExpr(3))))->pretty_print(out);
    CHECK(out.str() == "1 + 2 * 3");
}