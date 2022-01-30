//
// Created by Kelan Albertson on 1/28/22.
//

#include "_let.h"
#include "add.h"
#include "mult.h"
#include "num.h"
#include "var.h"
#include "catch.h"
#include <sstream>


_let::_let(Var* lhs, Expr* rhs, Expr* body) {
    this->lhs_ = lhs;
    this->rhs_ = rhs;
    this->body_ = body;
}

bool _let::equals(Expr* other) {
    _let* l = dynamic_cast<_let*>(other);
    if (l == nullptr) {
        return false;
    }
    else {
        return (this->lhs_->equals(l->lhs_) && this->rhs_->equals(l->rhs_) && this->body_->equals(l->body_));
    }
}

int _let::interp() {
    return 0;
    //TODO
}

bool _let::has_variable() {
    return (this->rhs_->has_variable() || this->body_->has_variable());
}

Expr *_let::subst(std::string variableName, Expr *replacement) {
    // rules from lecture videos:
    // if bind same variable then do not substitute in body
    // if bind different variable then do substitute in body
    // always substitute in rhs
    if (this->lhs_->name_ == variableName) {
        return new _let(this->lhs_,this->rhs_->subst(variableName, replacement), this->body_);
    }
    else {
        return new _let(this->lhs_,
                        this->rhs_->subst(variableName, replacement),
                        this->body_->subst(variableName, replacement));
    }
}

void _let::print(std::ostream &out) {
    out << "(_let ";
    this->lhs_->print(out);
    out << "=";
    this->rhs_->print(out);
    out << " _in ";
    this->body_->print(out);
    out << ")";
}

void _let::pretty_print_at(std::ostream &out, Expr::precedence_t prec) {
    //TODO
}

/**
 *************************   TESTS   **************************
 **/

TEST_CASE("_let equals() tests") {
    CHECK((new _let(new Var("x"), new Num(1), new Num(0)))->equals(new _let(new Var("x"), new Num(1), new Num(0))) == true);
    CHECK((new _let(new Var("x"), new Num(1), new Num(0)))->equals(new _let(new Var("y"), new Num(13), new Num(-2))) == false);
    CHECK((new _let(new Var("x"), new Num(5), new Add(new Var ("x"), new Num(1))))
            ->equals(new _let(new Var("x"), new Num(5), new Add(new Var ("x"), new Num(1)))) == true);
    CHECK((new _let(new Var("x"), new Num(5), new Add(new Var ("x"), new Num(1))))
                  ->equals(new _let(new Var("x"), new Num(5), new Add(new Var ("a"), new Num(1)))) == false);
    CHECK((new _let(new Var("x"), new Num(1), new Num(0)))->equals(new Var("x")) == false);
}

TEST_CASE("_let interp() tests") {

//    CHECK((new Add(new Num(0), new Num(0)))->interp() == 0);
//    CHECK((new Add(new Num(0), new Num(1)))->interp() == 1);
//    CHECK((new Add(new Num(1), new Num(0)))->interp() == 1);
//    CHECK((new Add(new Num(-5), new Num(18)))->interp() == 13);
}

TEST_CASE("_let has_variable() tests") {
    CHECK((new _let(new Var("x"), new Num(1), new Add(new Var("x"), new Num(1))))->has_variable() == true);
    CHECK((new _let(new Var("x"), new Add(new Var("y"), new Num(1)), new Num(1)))->has_variable() == true);
    CHECK((new _let(new Var("x"), new Num(1), new Num(0)))->has_variable() == false);
}

TEST_CASE("_let subst() tests") {
    CHECK((new _let(new Var("x"), new Num(6), new Add(new Var("x"), new Num(1))))->subst("x", new Num(5))
                ->equals(new _let(new Var("x"), new Num(6), new Add(new Var("x"), new Num(1)))));
    CHECK((new _let(new Var("y"), new Num(6), new Add(new Var("x"), new Num(1))))->subst("x", new Num(5))
                  ->equals(new _let(new Var("y"), new Num(6), new Add(new Num(5), new Num(1)))));
    CHECK((new _let(new Var("x"), new Add(new Var("x"), new Num(2)), new Add(new Var("x"), new Num(1))))->subst("x", new Num(5))
                  ->equals(new _let(new Var("x"), new Add(new Num(5), new Num(2)), new Add(new Var("x"), new Num(1)))));
}

TEST_CASE("_let print()/to_string() tests") {
    CHECK((new _let(new Var("y"), new Num(3), new Add(new Var("y"), new Num(2))))->to_string() == "(_let y=3 _in (y+2))");
    CHECK((new _let(new Var("x"), new Num(5), new _let(new Var("y"), new Num(3), new Add(new Var("y"), new Num(2)))))
            ->to_string() == "(_let x=5 _in (_let y=3 _in (y+2)))");
    CHECK((new _let(new Var("x"), new Num(5), new Add(new _let(new Var("y"), new Num(3), new Add(new Var("y"), new Num(2))), new Var("x"))))
            ->to_string() == "(_let x=5 _in ((_let y=3 _in (y+2))+x))");
}

TEST_CASE("_let pretty_print() tests") {
//    std::stringstream out("");
//    (new Add(new Num(1), new Num(2)))->pretty_print(out);
//    CHECK(out.str() == "1 + 2");
//    out.str(std::string());
//    (new Add(new Add(new Num(1), new Num(2)), new Num(3)))->pretty_print(out);
//    CHECK(out.str() == "(1 + 2) + 3");
//    out.str(std::string());
//    (new Add(new Num(1), new Mult(new Num(2), new Num(3))))->pretty_print(out);
//    CHECK(out.str() == "1 + 2 * 3");
}