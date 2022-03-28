cmake_minimum_required(VERSION 3.20)
project(MSDscript)

set(CMAKE_CXX_STANDARD 14)

add_executable(MSDscript main.cpp cmdline.cpp Expr.cpp NumExpr.cpp AddExpr.cpp MultExpr.cpp VarExpr.cpp LetExpr.cpp parse.cpp Val.cpp Val.h NumVal.cpp NumVal.h BoolVal.cpp BoolVal.h BoolExpr.cpp BoolExpr.h EqExpr.cpp EqExpr.h IfExpr.cpp IfExpr.h FunExpr.cpp FunExpr.h CallExpr.cpp CallExpr.h FunVal.cpp FunVal.h pointer.h Env.cpp Env.h EmptyEnv.cpp EmptyEnv.h ExtendedEnv.cpp ExtendedEnv.h Step.cpp Step.h Cont.cpp Cont.h RightThenAddCont.cpp RightThenAddCont.h AddCont.cpp AddCont.h IfBranchCont.cpp IfBranchCont.h LetBodyCont.cpp LetBodyCont.h ArgThenCallCont.cpp ArgThenCallCont.h CallCont.cpp CallCont.h RightThenMultCont.cpp RightThenMultCont.h MultCont.cpp MultCont.h RightThenCompCont.cpp RightThenCompCont.h CompCont.cpp CompCont.h)
add_executable(test_msdscript randomTests.cpp exec.cpp)
