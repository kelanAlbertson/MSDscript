//
// Created by Kelan Albertson on 1/16/22.
//

#include "add.h"

Add::Add(Expr* lhs, Expr* rhs) {
    this->lhs_ = lhs;
    this->rhs_ = rhs;
}

bool Add::equals(Expr* other) {
    Add* a = dynamic_cast<Add*>(other);
    if (a == NULL) {
        return false;
    }
    else {
        return (this->lhs_->equals(a->lhs_) && this->rhs_->equals(a->rhs_));
    }
}
