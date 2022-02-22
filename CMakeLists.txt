cmake_minimum_required(VERSION 3.20)
project(MSDscript)

set(CMAKE_CXX_STANDARD 14)

add_executable(MSDscript main.cpp cmdline.cpp Expr.cpp NumExpr.cpp AddExpr.cpp MultExpr.cpp VarExpr.cpp LetExpr.cpp parse.cpp Val.cpp Val.h NumVal.cpp NumVal.h BoolVal.cpp BoolVal.h BoolExpr.cpp BoolExpr.h EqExpr.cpp EqExpr.h IfExpr.cpp IfExpr.h)
add_executable(test_msdscript randomTests.cpp exec.cpp)
