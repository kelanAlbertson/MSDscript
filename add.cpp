//
// Created by Kelan Albertson on 1/16/22.
//

#include <sstream>
#include "add.h"
#include "catch.h"
#include "num.h"
#include "var.h"
#include "mult.h"

Add::Add(Expr* lhs, Expr* rhs) {
    this->lhs_ = lhs;
    this->rhs_ = rhs;
}

bool Add::equals(Expr* other) {
    Add* a = dynamic_cast<Add*>(other);
    if (a == nullptr) {
        return false;
    }
    else {
        return (this->lhs_->equals(a->lhs_) && this->rhs_->equals(a->rhs_));
    }
}

int Add::interp() {
    return (this->lhs_->interp() + this->rhs_->interp());
}

bool Add::has_variable() {
    return (this->lhs_->has_variable() || this->rhs_->has_variable());
}

Expr* Add::subst(std::string variableName, Expr *replacement) {
    return new Add(this->lhs_->subst(variableName, replacement),
                   this->rhs_->subst(variableName, replacement));
}

void Add::print(std::ostream &out) {
    out << "(";
    this->lhs_->print(out);
    out << "+";
    this->rhs_->print(out);
    out << ")";
}

TEST_CASE("Add equals() tests") {
    CHECK((new Add(new Num(0), new Num(1)))->equals(new Add(new Num(0), new Num(1))) == true);
    CHECK((new Add(new Num(0), new Num(1)))->equals(new Add(new Num(1), new Num(0))) == false);
    CHECK((new Add(new Num(0), new Num(1)))->equals(new Add(new Num(-20), new Num(9))) == false);
    CHECK((new Add(new Num(0), new Num(1)))->equals(new Num(1)) == false);
}

TEST_CASE("Add interp() tests") {
    CHECK((new Add(new Num(0), new Num(0)))->interp() == 0);
    CHECK((new Add(new Num(0), new Num(1)))->interp() == 1);
    CHECK((new Add(new Num(1), new Num(0)))->interp() == 1);
    CHECK((new Add(new Num(-5), new Num(18)))->interp() == 13);
}

TEST_CASE("Add has_variable() tests") {
    CHECK((new Add(new Num(0), new Num(1)))->has_variable() == false);
    CHECK((new Add(new Var("test"), new Num(1)))->has_variable() == true);
    CHECK((new Add(new Var("test"), new Var("test")))->has_variable() == true);
}

TEST_CASE("Add subst() tests") {
    CHECK((new Add(new Var("x"), new Num(7)))->subst("x", new Var("y"))
            ->equals(new Add(new Var("y"), new Num(7))));
    CHECK((new Add(new Var("a"), new Num(7)))->subst("x", new Var("y"))
            ->equals(new Add(new Var("a"), new Num(7))));

    CHECK((new Add(new Var("x"), new Add(new Mult(new Num(-1), new Var("x")), new Var("a"))))->subst("x", new Var("y"))
            -> equals(new Add(new Var("y"), new Add(new Mult(new Num(-1), new Var("y")), new Var("a")))));
}

TEST_CASE("Add print()/to_string() tests") {
    CHECK((new Add(new Num(1), new Num(2)))->to_string() == "(1+2)");
    CHECK((new Add(new Num(1), new Add(new Num(2), new Num(3))))->to_string() == "(1+(2+3))");
    CHECK((new Add(new Add(new Num(1), new Num(2)), new Num(3)))->to_string() == "((1+2)+3)");
}