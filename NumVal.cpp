//
// Created by Kelan Albertson on 2/16/22.
//

#include <stdexcept>
#include "NumVal.h"

NumVal::NumVal(int rep) {
    this->rep_ = rep;
}

bool NumVal::equals(Val* other) {
    NumVal *nv = dynamic_cast<NumVal*>(other);
    if (nv == nullptr) {
        return false;
    }
    else {
        return (this->rep_ == nv->rep_);
    }
}

std::string NumVal::to_string() {
    return std::to_string(this->rep_);
}

Expr *NumVal::to_expr() {
    return nullptr;
}

Val *NumVal::add_to(Val *other) {
    NumVal *nv = dynamic_cast<NumVal*>(other);
    if (nv == nullptr) {
        throw std::runtime_error("cannot add with a non_number");
    }
    return new NumVal(rep_ + nv->rep_);
}

Val *NumVal::multiply_by(Val *other) {
    NumVal *nv = dynamic_cast<NumVal*>(other);
    if (nv == nullptr) {
        throw std::runtime_error("cannot multiply with a non_number");
    }
    return new NumVal(rep_ * nv->rep_);
}
