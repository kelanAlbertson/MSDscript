//
// Created by Kelan Albertson on 1/17/22.
//

#include "var.h"

Var::Var(std::string name, int value) {
    this->name_ = name;
    this->value_ = value;
}

bool Var::equals(Expr *other) {
    Var* v = dynamic_cast<Var*>(other);
    if (v == NULL) {
        return false;
    }
    else {
        return (this->name_->equals(v->name_) && this->value_->equals(v->value_));
    }
}
