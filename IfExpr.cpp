//
// Created by Kelan Albertson on 2/20/22.
//

#include "IfExpr.h"
#include "Val.h"
#include <sstream>

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

void IfExpr::pretty_print_at(std::ostream &out, Expr::precedence_t prec, bool let_parentheses,
                             std::streampos &last_new_line_pos) {

}
