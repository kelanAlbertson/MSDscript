//
// Created by Kelan Albertson on 3/20/22.
//

#include <string>
#include "EmptyEnv.h"
#include "ExtendedEnv.h"
#include "NumVal.h"
#include "catch.h"

bool EmptyEnv::equals(std::shared_ptr<Env> other) {
    PTR(EmptyEnv) ee = CAST(EmptyEnv)(other);
    if (ee == nullptr) {
        return false;
    }
    else {
        return true;
    }
}

PTR(Val) EmptyEnv::lookup(std::string find_name) {
    throw std::runtime_error("free variable: " + find_name);
}

/**
 *************************   TESTS   **************************
 **/

TEST_CASE("EmptyEnv equals() tests") {
    CHECK((NEW(EmptyEnv)())->equals(NEW(EmptyEnv)()) == true);
    CHECK((NEW(EmptyEnv)())->equals(NEW(ExtendedEnv)("x", NEW(NumVal)(1), NEW(EmptyEnv)())) == false);
}

TEST_CASE("EmptyEnv lookup() tests") {
    CHECK_THROWS_WITH((NEW(EmptyEnv)())->lookup("x"), "free variable: x");
}
