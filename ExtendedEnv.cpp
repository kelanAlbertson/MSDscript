//
// Created by Kelan Albertson on 3/20/22.
//

#include <string>
#include "ExtendedEnv.h"
#include "EmptyEnv.h"
#include "Val.h"
#include "NumVal.h"
#include "catch.h"

ExtendedEnv::ExtendedEnv(std::string name, PTR(Val) val, PTR(Env) rest) {
    name_ = name;
    val_ = val;
    rest_ = rest;
}

bool ExtendedEnv::equals(std::shared_ptr<Env> other) {
    PTR(ExtendedEnv) ee = CAST(ExtendedEnv)(other);
    if (ee == nullptr) {
        return false;
    }
    else {
        return (this->name_ == ee->name_ && this->val_->equals(ee->val_) && this->rest_->equals(ee->rest_));
    }
}

PTR(Val) ExtendedEnv::lookup(std::string find_name) {
    if (find_name == name_) {
        return val_;
    }
    else {
        return rest_->lookup(find_name);
    }
}

/**
 *************************   TESTS   **************************
 **/

TEST_CASE("ExtendedEnv equals() tests") {
    CHECK((NEW(ExtendedEnv)("x", NEW(NumVal)(1), Env::empty))->equals(NEW(ExtendedEnv)("x", NEW(NumVal)(1), Env::empty)) == true);
    CHECK((NEW(ExtendedEnv)("x", NEW(NumVal)(1), NEW(ExtendedEnv)("x", NEW(NumVal)(99), Env::empty)))->equals(NEW(ExtendedEnv)("x", NEW(NumVal)(1), Env::empty)) == false);
}

TEST_CASE("ExtendedEnv lookup() tests") {
    CHECK_THROWS_WITH((NEW(ExtendedEnv)("x", NEW(NumVal)(1), NEW(ExtendedEnv)("y", NEW(NumVal)(99), Env::empty)))->lookup("b"), "free variable: b");
    CHECK((NEW(ExtendedEnv)("x", NEW(NumVal)(1), NEW(ExtendedEnv)("y", NEW(NumVal)(99), Env::empty)))->lookup("y")->equals(NEW(NumVal)(99)));
}
