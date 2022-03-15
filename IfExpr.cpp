//
// Created by Kelan Albertson on 2/20/22.
//

#include "IfExpr.h"
#include "Val.h"
#include "AddExpr.h"
#include "NumExpr.h"
#include "MultExpr.h"
#include "VarExpr.h"
#include "catch.h"
#include "EqExpr.h"
#include "BoolExpr.h"
#include "BoolVal.h"
#include "NumVal.h"
#include "LetExpr.h"
#include <sstream>
#include <string>

IfExpr::IfExpr(PTR(Expr) condition, PTR(Expr) ts, PTR(Expr) es) {
    this->condition_ = condition;
    this->then_statement_ = ts;
    this->else_statement_ = es;
}

bool IfExpr::equals(PTR(Expr) other) {
    PTR(IfExpr) i = CAST(IfExpr)(other);
    if (i == nullptr) {
        return false;
    }
    else {
        return (this->condition_->equals(i->condition_)
                && this->then_statement_->equals(i->then_statement_)
                && this->else_statement_->equals(i->else_statement_));
    }
}

PTR(Val)IfExpr::interp() {
    if (this->condition_->interp()->is_true()) {
        return this->then_statement_->interp();
    }
    else {
        return this->else_statement_->interp();
    }
}

//bool IfExpr::has_variable() {
//    return (this->condition_->has_variable() || this->then_statement_->has_variable() || this->else_statement_->has_variable());
//}

PTR(Expr)IfExpr::subst(std::string variableName, PTR(Expr) replacement) {
    return NEW(IfExpr)(this->condition_->subst(variableName, replacement),
                      this->then_statement_->subst(variableName, replacement),
                      this->else_statement_->subst(variableName, replacement));
}

void IfExpr::print(std::ostream &out) {
    out << "(_if ";
    this->condition_->print(out);
    out << " _then ";
    this->then_statement_->print(out);
    out << " _else ";
    this->else_statement_->print(out);
    out << ")";
}

void IfExpr::pretty_print_at(std::ostream &out, Expr::precedence_t prec, bool keyword_parentheses, std::streampos &last_new_line_pos) {
    if (keyword_parentheses) {
        out << "(";
    }

    int if_indent = out.tellp() - last_new_line_pos;
    out << "_if ";
    this->condition_->pretty_print_at(out, prec_none, false, last_new_line_pos);
    out << "\n";
    last_new_line_pos = out.tellp();

    for (int i = 0; i < if_indent; ++i) {
        out << " ";
    }
    out << "_then ";
    this->then_statement_->pretty_print_at(out, prec_none, false, last_new_line_pos);
    out << "\n";
    last_new_line_pos = out.tellp();

    for (int i = 0; i < if_indent; ++i) {
        out << " ";
    }
    out << "_else ";
    this->else_statement_->pretty_print_at(out, prec_none, false, last_new_line_pos);

    if (keyword_parentheses) {
        out << ")";
    }
}

/**
 *************************   TESTS   **************************
 **/

TEST_CASE("IfExpr equals() tests") {
    CHECK((NEW(IfExpr)(NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(BoolExpr)(true), NEW(BoolExpr)(false)))
            ->equals(NEW(IfExpr)(NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(BoolExpr)(true), NEW(BoolExpr)(false))) == true);
    CHECK((NEW(IfExpr)(NEW(BoolExpr)(false), NEW(NumExpr)(0), NEW(VarExpr)("y")))
                  ->equals(NEW(IfExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(0), NEW(VarExpr)("y"))) == false);
    CHECK((NEW(IfExpr)(NEW(EqExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("x")), NEW(NumExpr)(1), NEW(NumExpr)(0)))
                  ->equals(NEW(EqExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("x"))) == false);
}

TEST_CASE("IfExpr interp() tests") {
    CHECK((NEW(IfExpr)(NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(BoolExpr)(true), NEW(BoolExpr)(false)))
            ->interp()->equals(NEW(BoolVal)(false)));
    CHECK_THROWS_WITH((NEW(IfExpr)(NEW(BoolExpr)(false), NEW(NumExpr)(0), NEW(VarExpr)("y")))->interp(), "VarExpr cannot be interpreted");
    CHECK((NEW(IfExpr)(NEW(EqExpr)(NEW(AddExpr)(NEW(NumExpr)(3), NEW(NumExpr)(1)), NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(2))), NEW(NumExpr)(25), NEW(NumExpr)(100)))->interp()->equals(NEW(NumVal)(25)));
    CHECK((NEW(IfExpr)(NEW(BoolExpr)(false), NEW(NumExpr)(-1), NEW(BoolExpr)(false)))->interp()->equals(NEW(BoolVal)(false)));
    CHECK_THROWS_WITH((NEW(IfExpr)(NEW(NumExpr)(7), NEW(AddExpr)(NEW(VarExpr)("a"), NEW(VarExpr)("b")), NEW(NumExpr)(0)))->interp(), "Cannot use is_true() on a NumVal");
}

//TEST_CASE("IfExpr has_variable() tests") {
//    CHECK((new IfExpr(new EqExpr(new NumExpr(0), new NumExpr(1)), new VarExpr("x"), new VarExpr("y")))->has_variable() == true);
//    CHECK((new IfExpr(new BoolExpr(true), new NumExpr(1), new NumExpr(2)))->has_variable() == false);
//}

TEST_CASE("IfExpr subst() tests") {
    CHECK((NEW(IfExpr)(NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(BoolExpr)(true), NEW(BoolExpr)(false)))
            ->subst("x", NEW(BoolExpr)(false))->equals(NEW(IfExpr)(NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(BoolExpr)(true), NEW(BoolExpr)(false))));
    CHECK((NEW(IfExpr)(NEW(EqExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2)), NEW(BoolExpr)(true), NEW(BoolExpr)(false)))
            ->subst("x", NEW(NumExpr)(2))->equals(NEW(IfExpr)(NEW(EqExpr)(NEW(NumExpr)(2), NEW(NumExpr)(2)), NEW(BoolExpr)(true), NEW(BoolExpr)(false))));
    CHECK((NEW(IfExpr)(NEW(VarExpr)("test"), NEW(BoolExpr)(true), NEW(BoolExpr)(false)))
            ->subst("test", NEW(EqExpr)(NEW(BoolExpr)(true), NEW(BoolExpr)(false)))
            ->equals(NEW(IfExpr)(NEW(EqExpr)(NEW(BoolExpr)(true), NEW(BoolExpr)(false)), NEW(BoolExpr)(true), NEW(BoolExpr)(false))));
}

TEST_CASE("IfExpr print()/to_string() tests") {
    CHECK((NEW(IfExpr)(NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(BoolExpr)(true), NEW(BoolExpr)(false)))
            ->to_string() == "(_if (1==2) _then _true _else _false)");
    CHECK((NEW(IfExpr)(NEW(BoolExpr)(false), NEW(AddExpr)(NEW(NumExpr)(0), NEW(NumExpr)(0)), NEW(EqExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(1))))
            ->to_string() == "(_if _false _then (0+0) _else (y==1))");
    CHECK((NEW(IfExpr)(NEW(IfExpr)(NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(1)), NEW(BoolExpr)(true), NEW(BoolExpr)(false)), NEW(NumExpr)(10), NEW(NumExpr)(20)))
            ->to_string() == "(_if (_if (1==1) _then _true _else _false) _then 10 _else 20)");
}

TEST_CASE("IfExpr pretty_print() tests") {
    CHECK((NEW(IfExpr)(NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(BoolExpr)(true), NEW(BoolExpr)(false)))
            ->to_pretty_string() == "_if 1 == 2\n"
                                    "_then _true\n"
                                    "_else _false");
    CHECK((NEW(IfExpr)(NEW(BoolExpr)(false), NEW(AddExpr)(NEW(NumExpr)(0), NEW(NumExpr)(0)), NEW(EqExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(1))))
            ->to_pretty_string() == "_if _false\n"
                                    "_then 0 + 0\n"
                                    "_else y == 1");
    CHECK((NEW(IfExpr)(NEW(IfExpr)(NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(1)), NEW(BoolExpr)(true), NEW(BoolExpr)(false)), NEW(NumExpr)(10), NEW(NumExpr)(20)))
            ->to_pretty_string() == "_if _if 1 == 1\n"
                                    "    _then _true\n"
                                    "    _else _false\n"
                                    "_then 10\n"
                                    "_else 20");
    CHECK((NEW(AddExpr)(NEW(IfExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(1), NEW(NumExpr)(0)), NEW(NumExpr)(5)))
            ->to_pretty_string() == "(_if _true\n"
                                    " _then 1\n"
                                    " _else 0) + 5");
}