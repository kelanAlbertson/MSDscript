//
// Created by Kelan Albertson on 1/16/22.
//

#include "mult.h"
#include "add.h"
#include "num.h"
#include "var.h"
#include "catch.h"
#include <sstream>

Mult::Mult(Expr *lhs, Expr *rhs) {
    this->lhs_ = lhs;
    this->rhs_ = rhs;
}

bool Mult::equals(Expr *other) {
    Mult* m = dynamic_cast<Mult*>(other);
    if (m == nullptr) {
        return false;
    }
    else {
        return (this->lhs_->equals(m->lhs_) && this->rhs_->equals(m->rhs_));
    }
}

int Mult::interp() {
    return (this->lhs_->interp() * this->rhs_->interp());
}

bool Mult::has_variable() {
    return (this->lhs_->has_variable() || this->rhs_->has_variable());
}

Expr* Mult::subst(std::string variableName, Expr *replacement) {
    return new Mult(this->lhs_->subst(variableName, replacement),
            this->rhs_->subst(variableName, replacement));
}

void Mult::print(std::ostream &out) {
    out << "(";
    this->lhs_->print(out);
    out << "*";
    this->rhs_->print(out);
    out << ")";
}

void Mult::pretty_print_at(std::ostream &out, Expr::precedence_t prec) {
    if (prec == prec_mult) {
        out << "(";
    }

    this->lhs_->pretty_print_at(out, prec_mult);
    out << " * ";
    this->rhs_->pretty_print_at(out, prec_add);

    if (prec == prec_mult) {
        out << ")";
    }
}

/**
 *************************   TESTS   **************************
 **/

TEST_CASE("Mult equals() tests") {
    CHECK((new Mult(new Num(-1), new Num(0)))->equals(new Mult(new Num(-1), new Num(0))) == true);
    CHECK((new Mult(new Num(-1), new Num(0)))->equals(new Mult(new Num(0), new Num(-1))) == false);
    CHECK((new Mult(new Num(-1), new Num(0)))->equals(new Mult(new Num(100), new Num(0))) == false);
    CHECK((new Mult(new Num(-1), new Num(0)))->equals(new Add(new Num(-1), new Num(0))) == false);
}

TEST_CASE("Mult interp() tests") {
    CHECK((new Mult(new Num(0), new Num(0)))->interp() == 0);
    CHECK((new Mult(new Num(0), new Num(1)))->interp() == 0);
    CHECK((new Mult(new Num(-1), new Num(10)))->interp() == -10);
    CHECK((new Mult(new Num(-5), new Num(-5)))->interp() == 25);
}

TEST_CASE("Mult has_variable() tests") {
    CHECK((new Mult(new Num(0), new Num(1)))->has_variable() == false);
    CHECK((new Mult(new Var("test"), new Num(1)))->has_variable() == true);
    CHECK((new Mult(new Var("test"), new Var("test")))->has_variable() == true);
}

TEST_CASE("Mult subst() tests") {
    CHECK((new Mult(new Var("x"), new Var("x")))->subst("x", new Var("y"))
            ->equals(new Mult(new Var("y"), new Var("y"))));
    CHECK((new Mult(new Var("a"), new Var("x")))->subst("x", new Var("y"))
            ->equals(new Mult(new Var("a"), new Var("y"))));
}

TEST_CASE("Mult print() tests") {
    CHECK((new Mult(new Num(1), new Num(2)))->to_string() == "(1*2)");
    CHECK((new Mult(new Add(new Var("test"), new Num(0)), new Num(1)))->to_string() == "((test+0)*1)");
}

TEST_CASE("Mult pretty_print() tests") {
    std::stringstream out("");
    (new Mult(new Num(1), new Num(2)))->pretty_print(out);
    CHECK(out.str() == "1 * 2");
    out.str(std::string());
    (new Mult(new Mult(new Num(2), new Num(3)), new Num(4)))->pretty_print(out);
    CHECK(out.str() == "(2 * 3) * 4");
    out.str(std::string());
    (new Mult(new Num(2), new Mult(new Num(3), new Num(4))))->pretty_print(out);
    CHECK(out.str() == "2 * 3 * 4");
    out.str(std::string());
    (new Mult(new Add(new Num(1), new Num(2)), new Add(new Num(3), new Num(4))))
            ->pretty_print(out);
    CHECK(out.str() == "(1 + 2) * (3 + 4)");
}