//
// Created by Kelan Albertson on 3/28/22.
//

#include "LetBodyCont.h"
#include "Step.h"
#include "ExtendedEnv.h"

LetBodyCont::LetBodyCont(std::string var, PTR(Expr) body, PTR(Env) env, PTR(Cont) rest) {
    this->var_ = var;
    this->body_ = body;
    this->env_ = env;
    this->rest_ = rest;
}

void LetBodyCont::step_continue() {
    Step::mode_ = Step::interp_mode;
    Step::expr_ = body_;
    Step::env_ = NEW(ExtendedEnv)(var_, Step::val_, env_);
    Step::cont_ = rest_;
}

