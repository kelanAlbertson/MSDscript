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

IfExpr::IfExpr(Expr *condition, Expr *ts, Expr *es) {
    this->condition_ = condition;
    this->then_statement_ = ts;
    this->else_statement_ = es;
}

bool IfExpr::equals(Expr *other) {
    IfExpr* i = dynamic_cast<IfExpr*>(other);
    if (i == nullptr) {
        return false;
    }
    else {
        return (this->condition_->equals(i->condition_)
                && this->then_statement_->equals(i->then_statement_)
                && this->else_statement_->equals(i->else_statement_));
    }
}

Val *IfExpr::interp() {
    if (this->condition_->interp()->is_true()) {
        return this->then_statement_->interp();
    }
    else {
        return this->else_statement_->interp();
    }
}

bool IfExpr::has_variable() {
    return (this->condition_->has_variable() || this->then_statement_->has_variable() || this->else_statement_->has_variable());
}

Expr *IfExpr::subst(std::string variableName, Expr *replacement) {
    return new IfExpr(this->condition_->subst(variableName, replacement),
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
    CHECK((new IfExpr(new EqExpr(new NumExpr(1), new NumExpr(2)), new BoolExpr(true), new BoolExpr(false)))
            ->equals(new IfExpr(new EqExpr(new NumExpr(1), new NumExpr(2)), new BoolExpr(true), new BoolExpr(false))) == true);
    CHECK((new IfExpr(new BoolExpr(false), new NumExpr(0), new VarExpr("y")))
                  ->equals(new IfExpr(new BoolExpr(true), new NumExpr(0), new VarExpr("y"))) == false);
    CHECK((new IfExpr(new EqExpr(new VarExpr("x"), new VarExpr("x")), new NumExpr(1), new NumExpr(0)))
                  ->equals(new EqExpr(new VarExpr("x"), new VarExpr("x"))) == false);
}

TEST_CASE("IfExpr interp() tests") {
    CHECK((new IfExpr(new EqExpr(new NumExpr(1), new NumExpr(2)), new BoolExpr(true), new BoolExpr(false)))
            ->interp()->equals(new BoolVal(false)));
    CHECK_THROWS_WITH((new IfExpr(new BoolExpr(false), new NumExpr(0), new VarExpr("y")))->interp(), "VarExpr cannot be interpreted");
    CHECK((new IfExpr(new EqExpr(new AddExpr(new NumExpr(3), new NumExpr(1)), new AddExpr(new NumExpr(2), new NumExpr(2))), new NumExpr(25), new NumExpr(100)))->interp()->equals(new NumVal(25)));
    CHECK((new IfExpr(new BoolExpr(false), new NumExpr(-1), new BoolExpr(false)))->interp()->equals(new BoolVal(false)));
    CHECK_THROWS_WITH((new IfExpr(new NumExpr(7), new AddExpr(new VarExpr("a"), new VarExpr("b")), new NumExpr(0)))->interp(), "Cannot use is_true() on a NumVal");
}

TEST_CASE("IfExpr has_variable() tests") {
    CHECK((new IfExpr(new EqExpr(new NumExpr(0), new NumExpr(1)), new VarExpr("x"), new VarExpr("y")))->has_variable() == true);
    CHECK((new IfExpr(new BoolExpr(true), new NumExpr(1), new NumExpr(2)))->has_variable() == false);
}

TEST_CASE("IfExpr subst() tests") {
    CHECK((new IfExpr(new EqExpr(new NumExpr(1), new NumExpr(2)), new BoolExpr(true), new BoolExpr(false)))
            ->subst("x", new BoolExpr(false))->equals(new IfExpr(new EqExpr(new NumExpr(1), new NumExpr(2)), new BoolExpr(true), new BoolExpr(false))));
    CHECK((new IfExpr(new EqExpr(new VarExpr("x"), new NumExpr(2)), new BoolExpr(true), new BoolExpr(false)))
            ->subst("x", new NumExpr(2))->equals(new IfExpr(new EqExpr(new NumExpr(2), new NumExpr(2)), new BoolExpr(true), new BoolExpr(false))));
    CHECK((new IfExpr(new VarExpr("test"), new BoolExpr(true), new BoolExpr(false)))
            ->subst("test", new EqExpr(new BoolExpr(true), new BoolExpr(false)))
            ->equals(new IfExpr(new EqExpr(new BoolExpr(true), new BoolExpr(false)), new BoolExpr(true), new BoolExpr(false))));
}

TEST_CASE("IfExpr print()/to_string() tests") {
    CHECK((new IfExpr(new EqExpr(new NumExpr(1), new NumExpr(2)), new BoolExpr(true), new BoolExpr(false)))
            ->to_string() == "(_if (1==2) _then _true _else _false)");
    CHECK((new IfExpr(new BoolExpr(false), new AddExpr(new NumExpr(0), new NumExpr(0)), new EqExpr(new VarExpr("y"), new NumExpr(1))))
            ->to_string() == "(_if _false _then (0+0) _else (y==1))");
    CHECK((new IfExpr(new IfExpr(new EqExpr(new NumExpr(1), new NumExpr(1)), new BoolExpr(true), new BoolExpr(false)), new NumExpr(10), new NumExpr(20)))
            ->to_string() == "(_if (_if (1==1) _then _true _else _false) _then 10 _else 20)");
}

TEST_CASE("IfExpr pretty_print() tests") {
    CHECK((new IfExpr(new EqExpr(new NumExpr(1), new NumExpr(2)), new BoolExpr(true), new BoolExpr(false)))
                  ->to_pretty_string() == "_if 1 == 2\n"
                                          "_then _true\n"
                                          "_else _false");
    CHECK((new IfExpr(new BoolExpr(false), new AddExpr(new NumExpr(0), new NumExpr(0)), new EqExpr(new VarExpr("y"), new NumExpr(1))))
                  ->to_pretty_string() == "_if _false\n"
                                          "_then 0 + 0\n"
                                          "_else y == 1");
    CHECK((new IfExpr(new IfExpr(new EqExpr(new NumExpr(1), new NumExpr(1)), new BoolExpr(true), new BoolExpr(false)), new NumExpr(10), new NumExpr(20)))
                  ->to_pretty_string() == "_if _if 1 == 1\n"
                                          "    _then _true\n"
                                          "    _else _false\n"
                                          "_then 10\n"
                                          "_else 20");
}