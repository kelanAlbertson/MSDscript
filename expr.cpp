//
// Created by Kelan Albertson on 1/16/22.
//

#include <sstream>
#include "expr.h"

std::string Expr::to_string() {
    std::stringstream out("");
    this->print(out);
    return out.str();
}

