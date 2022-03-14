//
// Created by Kelan Albertson on 2/26/22.
//

#include "FunExpr.h"
#include "FunVal.h"
#include "VarExpr.h"
#include "catch.h"
#include "AddExpr.h"
#include "NumExpr.h"
#include "CallExpr.h"
#include "MultExpr.h"

FunExpr::FunExpr(VarExpr *arg, Expr *body) {
    this->arg_ = arg;
    this->body_ = body;
}

bool FunExpr::equals(Expr *other) {
    FunExpr *f = dynamic_cast<FunExpr*>(other);
    if (f == nullptr) {
        return false;
    }
    else {
        return (this->arg_->equals(f->arg_) && this->body_->equals(f->body_));
    }
}

Val *FunExpr::interp() {
    return new FunVal(this->arg_, this->body_);
}

//bool FunExpr::has_variable() {
//    return false;
//}

Expr *FunExpr::subst(std::string variableName, Expr *replacement) {
    if (this->arg_->name_ == variableName) {
        return this;
    }
    return new FunExpr(this->arg_, this->body_->subst(variableName, replacement));
}

void FunExpr::print(std::ostream &out) {
    out << "(_fun (";
    this->arg_->print(out);
    out << ") ";
    this->body_->print(out);
    out << ")";
}

void FunExpr::pretty_print_at(std::ostream &out, Expr::precedence_t prec, bool keyword_parentheses, std::streampos &last_new_line_pos) {
    if (keyword_parentheses) {
        out << "(";
    }

    int fun_indent = out.tellp() - last_new_line_pos;
    out << "_fun (" + this->arg_->name_ + ")\n";
    last_new_line_pos = out.tellp();

    for (int i = 0; i < fun_indent + 2; ++i) {
        out << " ";
    }

    this->body_->pretty_print_at(out, prec_none, false, last_new_line_pos);

    if (keyword_parentheses) {
        out << ")";
    }
}

/**
 *************************   TESTS   **************************
 **/

TEST_CASE("FunExpr equals() tests") {
    CHECK((new FunExpr(new VarExpr("x"), new AddExpr(new VarExpr("x"), new NumExpr(1))))
            ->equals(new FunExpr(new VarExpr("x"), new AddExpr(new VarExpr("x"), new NumExpr(1)))) == true);
    CHECK((new FunExpr(new VarExpr("x"), new AddExpr(new VarExpr("x"), new NumExpr(1))))
            ->equals(new FunExpr(new VarExpr("x"), new NumExpr(1))) == false);
    CHECK((new FunExpr(new VarExpr("x"), new AddExpr(new VarExpr("x"), new NumExpr(1))))
            ->equals(new CallExpr(new VarExpr("x"), new AddExpr(new VarExpr("x"), new NumExpr(1)))) == false);
}

TEST_CASE("FunExpr interp() tests") {
    CHECK((new FunExpr(new VarExpr("x"), new NumExpr(1)))
            ->interp()->equals(new FunVal(new VarExpr("x"), new NumExpr(1))));
    CHECK((new FunExpr(new VarExpr("x"), new AddExpr(new VarExpr("x"), new NumExpr(1))))
            ->interp()->equals(new FunVal(new VarExpr("x"), new AddExpr(new VarExpr("x"), new NumExpr(1)))));
    CHECK((new FunExpr(new VarExpr("y"), new MultExpr(new VarExpr("y"), new VarExpr("y"))))
            ->interp()->equals(new FunVal(new VarExpr("y"), new MultExpr(new VarExpr("y"), new VarExpr("y")))));
}

TEST_CASE("FunExpr subst() tests") {
    CHECK((new FunExpr(new VarExpr("x"), new MultExpr(new VarExpr("x"), new VarExpr("y"))))->subst("y", new NumExpr(8))
            ->equals(new FunExpr(new VarExpr("x"), new MultExpr(new VarExpr("x"), new NumExpr(8)))));
    CHECK((new FunExpr(new VarExpr("x"), new MultExpr(new VarExpr("x"), new VarExpr("x"))))->subst("x", new NumExpr(8))
            ->equals(new FunExpr(new VarExpr("x"), new MultExpr(new VarExpr("x"), new VarExpr("x")))));
    CHECK((new FunExpr(new VarExpr("x"), new AddExpr(new NumExpr(1), new NumExpr(1))))->subst("x", new NumExpr(8))
                  ->equals(new FunExpr(new VarExpr("x"), new AddExpr(new NumExpr(1), new NumExpr(1)))));
}

TEST_CASE("FunExpr print() tests") {
    CHECK((new FunExpr(new VarExpr("x"), new NumExpr(1)))->to_string() == "(_fun (x) 1)");
    CHECK((new FunExpr(new VarExpr("x"), new AddExpr(new VarExpr("x"), new NumExpr(1))))
            ->to_string() == "(_fun (x) (x+1))");
    CHECK((new FunExpr(new VarExpr("x"), new MultExpr(new VarExpr("x"), new VarExpr("y"))))
            ->to_string() == "(_fun (x) (x*y))");
    CHECK((new FunExpr(new VarExpr("x"), new FunExpr(new VarExpr("y"), new AddExpr(new MultExpr(new VarExpr("x"), new VarExpr("x")), new MultExpr(new VarExpr("y"), new VarExpr("y"))))))
            ->to_string() == "(_fun (x) (_fun (y) ((x*x)+(y*y))))");
}

TEST_CASE("FunExpr pretty_print() tests") {
    CHECK((new FunExpr(new VarExpr("x"), new NumExpr(1)))->to_pretty_string() == "_fun (x)\n"
                                                                                 "  1");
    CHECK((new FunExpr(new VarExpr("x"), new FunExpr(new VarExpr("y"), new AddExpr(new MultExpr(new VarExpr("x"), new VarExpr("x")), new MultExpr(new VarExpr("y"), new VarExpr("y"))))))
            ->to_pretty_string() == "_fun (x)\n"
                                   "  _fun (y)\n"
                                   "    x * x + y * y");
}
