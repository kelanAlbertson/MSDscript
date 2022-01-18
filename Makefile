INCS = cmdline.h
OBJS = main.o cmdline.o

msdscript : main.o cmdline.o expr.o num.o add.o mult.o var.o
	c++ --std=c++14 -O2 -o msdscript main.o cmdline.o expr.o num.o add.o mult.o var.o

main.o : main.cpp cmdline.h
	c++ --std=c++14 -O2 -c main.cpp

cmdline.o : cmdline.cpp catch.h cmdline.h
	c++ --std=c++14 -O2 -c cmdline.cpp

expr.o : expr.cpp expr.h
	c++ --std=c++14 -O2 -c expr.cpp

num.o : num.cpp num.h catch.h
	c++ --std=c++14 -O2 -c num.cpp

add.o : add.cpp add.h catch.h num.h
	c++ --std=c++14 -O2 -c add.cpp

mult.o : mult.cpp mult.h add.h catch.h num.h
	c++ --std=c++14 -O2 -c mult.cpp

var.o : var.cpp var.h catch.h num.h
	c++ --std=c++14 -O2 -c var.cpp

clean :
	rm msdscript main.o cmdline.o expr.o num.o add.o mult.o var.o
