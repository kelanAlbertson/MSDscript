//
// Created by Kelan Albertson on 3/20/22.
//

#include <string>
#include "ExtendedEnv.h"

PTR(Val) ExtendedEnv::lookup(std::string find_name) {
    if (find_name == name_) {
        return val_;
    }
    else {
        return rest_->lookup(find_name);
    }
}
