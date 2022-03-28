//
// Created by Kelan Albertson on 3/28/22.
//

#include "Cont.h"
#include "DoneCont.h"

PTR(Cont) Cont::done = NEW(DoneCont)();