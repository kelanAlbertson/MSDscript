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

FunExpr::FunExpr(PTR(VarExpr) arg, PTR(Expr) body) {
    this->arg_ = arg;
    this->body_ = body;
}

bool FunExpr::equals(PTR(Expr) other) {
    PTR(FunExpr)f = CAST(FunExpr)(other);
    if (f == nullptr) {
        return false;
    }
    else {
        return (this->arg_->equals(f->arg_) && this->body_->equals(f->body_));
    }
}

PTR(Val)FunExpr::interp() {
    return NEW(FunVal)(this->arg_, this->body_);
}

//bool FunExpr::has_variable() {
//    return false;
//}

PTR(Expr)FunExpr::subst(std::string variableName, PTR(Expr) replacement) {
    if (this->arg_->name_ == variableName) {
        return THIS;
    }
    return NEW(FunExpr)(this->arg_, this->body_->subst(variableName, replacement));
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
    CHECK((NEW(FunExpr)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))
            ->equals(NEW(FunExpr)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))) == true);
    CHECK((NEW(FunExpr)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))
            ->equals(NEW(FunExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))) == false);
    CHECK((NEW(FunExpr)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))
            ->equals(NEW(CallExpr)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))) == false);
}

TEST_CASE("FunExpr interp() tests") {
    CHECK((NEW(FunExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))
            ->interp()->equals(NEW(FunVal)(NEW(VarExpr)("x"), NEW(NumExpr)(1))));
    CHECK((NEW(FunExpr)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))
            ->interp()->equals(NEW(FunVal)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))));
    CHECK((NEW(FunExpr)(NEW(VarExpr)("y"), NEW(MultExpr)(NEW(VarExpr)("y"), NEW(VarExpr)("y"))))
            ->interp()->equals(NEW(FunVal)(NEW(VarExpr)("y"), NEW(MultExpr)(NEW(VarExpr)("y"), NEW(VarExpr)("y")))));
}

TEST_CASE("FunExpr subst() tests") {
    CHECK((NEW(FunExpr)(NEW(VarExpr)("x"), NEW(MultExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y"))))->subst("y", NEW(NumExpr)(8))
            ->equals(NEW(FunExpr)(NEW(VarExpr)("x"), NEW(MultExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(8)))));
    CHECK((NEW(FunExpr)(NEW(VarExpr)("x"), NEW(MultExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("x"))))->subst("x", NEW(NumExpr)(8))
            ->equals(NEW(FunExpr)(NEW(VarExpr)("x"), NEW(MultExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("x")))));
    CHECK((NEW(FunExpr)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(1))))->subst("x", NEW(NumExpr)(8))
                  ->equals(NEW(FunExpr)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(1)))));
}

TEST_CASE("FunExpr print() tests") {
    CHECK((NEW(FunExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))->to_string() == "(_fun (x) 1)");
    CHECK((NEW(FunExpr)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))
            ->to_string() == "(_fun (x) (x+1))");
    CHECK((NEW(FunExpr)(NEW(VarExpr)("x"), NEW(MultExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y"))))
            ->to_string() == "(_fun (x) (x*y))");
    CHECK((NEW(FunExpr)(NEW(VarExpr)("x"), NEW(FunExpr)(NEW(VarExpr)("y"), NEW(AddExpr)(NEW(MultExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("x")), NEW(MultExpr)(NEW(VarExpr)("y"), NEW(VarExpr)("y"))))))
            ->to_string() == "(_fun (x) (_fun (y) ((x*x)+(y*y))))");
}

TEST_CASE("FunExpr pretty_print() tests") {
    CHECK((NEW(FunExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))->to_pretty_string() == "_fun (x)\n"
                                                                                 "  1");
    CHECK((NEW(FunExpr)(NEW(VarExpr)("x"), NEW(FunExpr)(NEW(VarExpr)("y"), NEW(AddExpr)(NEW(MultExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("x")), NEW(MultExpr)(NEW(VarExpr)("y"), NEW(VarExpr)("y"))))))
            ->to_pretty_string() == "_fun (x)\n"
                                   "  _fun (y)\n"
                                   "    x * x + y * y");
}
