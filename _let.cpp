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
    return nullptr;
    //TODO
}

void _let::print(std::ostream &out) {
    //TODO
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

//    CHECK((new Add(new Var("x"), new Num(7)))->subst("x", new Var("y"))
//                  ->equals(new Add(new Var("y"), new Num(7))));
//    CHECK((new Add(new Var("a"), new Num(7)))->subst("x", new Var("y"))
//                  ->equals(new Add(new Var("a"), new Num(7))));
//
//    CHECK((new Add(new Var("x"), new Add(new Mult(new Num(-1), new Var("x")), new Var("a"))))->subst("x", new Var("y"))
//                  ->equals(new Add(new Var("y"), new Add(new Mult(new Num(-1), new Var("y")), new Var("a")))));
}

TEST_CASE("_let print()/to_string() tests") {

//    CHECK((new Add(new Num(1), new Num(2)))->to_string() == "(1+2)");
//    CHECK((new Add(new Num(1), new Add(new Num(2), new Num(3))))->to_string() == "(1+(2+3))");
//    CHECK((new Add(new Add(new Num(1), new Num(2)), new Num(3)))->to_string() == "((1+2)+3)");
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