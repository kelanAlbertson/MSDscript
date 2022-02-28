//
// Created by Kelan Albertson on 2/26/22.
//

#include "CallExpr.h"
#include "Val.h"
#include "catch.h"

CallExpr::CallExpr(Expr *to_be_called, Expr *arg) {
    this->to_be_called_ = to_be_called;
    this->arg_ = arg;
}

bool CallExpr::equals(Expr *other) {
    CallExpr *c = dynamic_cast<CallExpr*>(other);
    if (c == nullptr) {
        return false;
    }
    else {
        return (this->to_be_called_->equals(c->to_be_called_) && this->arg_->equals(c->arg_));
    }
}

Val *CallExpr::interp() {
    return to_be_called_->interp()->call(arg_->interp());
}

bool CallExpr::has_variable() {
    return false;
}

Expr *CallExpr::subst(std::string variableName, Expr *replacement) {
    return nullptr;
}

void CallExpr::print(std::ostream &out) {

}

void CallExpr::pretty_print_at(std::ostream &out, Expr::precedence_t prec, bool keyword_parentheses,
                               std::streampos &last_new_line_pos) {

}

/**
 *************************   TESTS   **************************
 **/
