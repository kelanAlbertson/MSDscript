//
// Created by Kelan Albertson on 3/20/22.
//

#include "Env.h"
#include "EmptyEnv.h"

PTR(Env) Env::empty = NEW(EmptyEnv)();