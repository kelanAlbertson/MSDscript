//
// Created by Kelan Albertson on 3/20/22.
//

#include <string>
#include "EmptyEnv.h"

PTR(Val) EmptyEnv::lookup(std::string find_name) {
    throw std::runtime_error("free variable: " + find_name);
}
