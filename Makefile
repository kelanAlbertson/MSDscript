INCS = cmdline.h
OBJS = main.o cmdline.o

msdscript: main.o cmdline.o Expr.o NumExpr.o AddExpr.o MultExpr.o VarExpr.o LetExpr.o Val.o NumVal.o parse.o BoolVal.o BoolExpr.o EqExpr.o IfExpr.o
	c++ --std=c++14 -O2 -o msdscript main.o cmdline.o Expr.o NumExpr.o AddExpr.o MultExpr.o VarExpr.o LetExpr.o Val.o NumVal.o parse.o BoolVal.o BoolExpr.o EqExpr.o IfExpr.o

test_msdscript: randomTests.o exec.o
	c++ --std=c++14 -O2 -o test_msdscript randomTests.o exec.o

.PHONY: test
test: msdscript
	./msdscript --test

main.o: main.cpp cmdline.h
	c++ --std=c++14 -O2 -c main.cpp

cmdline.o: cmdline.cpp catch.h cmdline.h
	c++ --std=c++14 -O2 -c cmdline.cpp

Expr.o: Expr.cpp Expr.h
	c++ --std=c++14 -O2 -c Expr.cpp

NumExpr.o: NumExpr.cpp NumExpr.h catch.h
	c++ --std=c++14 -O2 -c NumExpr.cpp

AddExpr.o: AddExpr.cpp AddExpr.h MultExpr.h NumExpr.h VarExpr.h catch.h
	c++ --std=c++14 -O2 -c AddExpr.cpp

MultExpr.o: MultExpr.cpp MultExpr.h AddExpr.h NumExpr.h VarExpr.h catch.h
	c++ --std=c++14 -O2 -c MultExpr.cpp

VarExpr.o: VarExpr.cpp VarExpr.h AddExpr.h NumExpr.h MultExpr.h catch.h
	c++ --std=c++14 -O2 -c VarExpr.cpp

LetExpr.o: LetExpr.cpp LetExpr.h AddExpr.h NumExpr.h MultExpr.h VarExpr.h catch.h
	c++ --std=c++14 -O2 -c LetExpr.cpp

Val.o: Val.cpp Val.h
	c++ --std=c++14 -O2 -c Val.cpp

NumVal.o: NumVal.cpp NumVal.h
	c++ --std=c++14 -O2 -c NumVal.cpp

parse.o: parse.cpp parse.h
	c++ --std=c++14 -O2 -c parse.cpp

randomTests.o: randomTests.cpp exec.h
	c++ --std=c++14 -O2 -c randomTests.cpp

exec.o: exec.cpp exec.h
	c++ --std=c++14 -O2 -c exec.cpp

BoolVal.o: BoolVal.cpp BoolVal.h
	c++ --std=c++14 -O2 -c BoolVal.cpp

BoolExpr.o: BoolExpr.cpp BoolExpr.h
	c++ --std=c++14 -O2 -c BoolExpr.cpp

EqExpr.o: EqExpr.cpp EqExpr.h
	c++ --std=c++14 -O2 -c EqExpr.cpp

IfExpr.o: IfExpr.cpp IfExpr.h
	c++ --std=c++14 -O2 -c IfExpr.cpp

clean:
	rm msdscript main.o cmdline.o Expr.o NumExpr.o AddExpr.o MultExpr.o VarExpr.o LetExpr.o Val.o NumVal.o parse.o test_msdscript randomTests.o exec.o BoolVal.o BoolExpr.o EqExpr.o IfExpr.o
