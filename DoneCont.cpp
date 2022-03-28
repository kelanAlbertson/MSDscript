//
// Created by Kelan Albertson on 3/28/22.
//

#include "DoneCont.h"

void DoneCont::step_continue() {
    throw std::runtime_error("Cannot step_continue() with a DoneCont");
}
