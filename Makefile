INCS = cmdline.h
OBJS = main.o cmdline.o

msdscript: main.o cmdline.o expr.o num.o add.o mult.o var.o _let.o parse.o
	c++ --std=c++14 -O2 -o msdscript main.o cmdline.o expr.o num.o add.o mult.o var.o _let.o parse.o

.PHONY: test
test: msdscript
	./msdscript --test

main.o: main.cpp cmdline.h
	c++ --std=c++14 -O2 -c main.cpp

cmdline.o: cmdline.cpp catch.h cmdline.h
	c++ --std=c++14 -O2 -c cmdline.cpp

expr.o: expr.cpp expr.h
	c++ --std=c++14 -O2 -c expr.cpp

num.o: num.cpp num.h catch.h
	c++ --std=c++14 -O2 -c num.cpp

add.o: add.cpp add.h mult.h num.h var.h catch.h
	c++ --std=c++14 -O2 -c add.cpp

mult.o: mult.cpp mult.h add.h num.h var.h catch.h
	c++ --std=c++14 -O2 -c mult.cpp

var.o: var.cpp var.h add.h num.h mult.h catch.h
	c++ --std=c++14 -O2 -c var.cpp

_let.o: _let.cpp _let.h add.h num.h mult.h var.h catch.h
	c++ --std=c++14 -O2 -c _let.cpp

parse.o: parse.cpp
	c++ --std=c++14 -O2 -c parse.cpp

clean:
	rm msdscript main.o cmdline.o expr.o num.o add.o mult.o var.o _let.o parse.o
