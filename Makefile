INCS = cmdline.h
OBJS = main.o cmdline.o

msdscript: main.o cmdline.o expr.o num.o add.o mult.o var.o _let.o parse.o
	c++ --std=c++14 -O2 -o msdscript main.o cmdline.o expr.o num.o add.o mult.o var.o _let.o parse.o

test_msdscript: randomTests.o exec.o
	c++ --std=c++14 -O2 -o test_msdscript randomTests.o exec.o

.PHONY: test
test: msdscript
	./msdscript --test

main.o: main.cpp cmdline.h
	c++ --std=c++14 -O2 -c main.cpp

cmdline.o: cmdline.cpp catch.h cmdline.h
	c++ --std=c++14 -O2 -c cmdline.cpp

expr.o: Expr.cpp Expr.h
	c++ --std=c++14 -O2 -c expr.cpp

num.o: NumExpr.cpp NumExpr.h catch.h
	c++ --std=c++14 -O2 -c num.cpp

add.o: AddExpr.cpp AddExpr.h MultExpr.h NumExpr.h VarExpr.h catch.h
	c++ --std=c++14 -O2 -c add.cpp

mult.o: MultExpr.cpp MultExpr.h AddExpr.h NumExpr.h VarExpr.h catch.h
	c++ --std=c++14 -O2 -c mult.cpp

var.o: VarExpr.cpp VarExpr.h AddExpr.h NumExpr.h MultExpr.h catch.h
	c++ --std=c++14 -O2 -c var.cpp

_let.o: LetExpr.cpp LetExpr.h AddExpr.h NumExpr.h MultExpr.h VarExpr.h catch.h
	c++ --std=c++14 -O2 -c _let.cpp

parse.o: parse.cpp parse.h
	c++ --std=c++14 -O2 -c parse.cpp

randomTests.o: randomTests.cpp exec.h
	c++ --std=c++14 -O2 -c randomTests.cpp

exec.o: exec.cpp exec.h
	c++ --std=c++14 -O2 -c exec.cpp

clean:
	rm msdscript main.o cmdline.o expr.o num.o add.o mult.o var.o _let.o parse.o test_msdscript randomTests.o exec.o
