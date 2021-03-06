//
// Created by Kelan Albertson on 1/16/22.
//

#include "Expr.h"
#include "catch.h"
#include "Val.h"
#include <sstream>

std::string Expr::to_string() {
    std::stringstream out("");
    this->print(out);
    return out.str();
}

void Expr::pretty_print(std::ostream &out) {
    std::streampos zero = out.tellp();
    pretty_print_at(out, prec_none, false, zero);
}

std::string Expr::to_pretty_string() {
    std::stringstream out("");
    this->pretty_print(out);
    return out.str();
}