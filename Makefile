INCS = cmdline.h

OBJS = main.o cmdline.o Expr.o NumExpr.o AddExpr.o MultExpr.o VarExpr.o LetExpr.o BoolExpr.o EqExpr.o IfExpr.o FunExpr.o CallExpr.o Val.o NumVal.o BoolVal.o FunVal.o Env.o EmptyEnv.o ExtendedEnv.o parse.o
TESTOBJS = randomTests.o exec.o

CXXFLAGS = --std=c++14 -O2

msdscript: $(OBJS)
	$(CXX) $(CXXFLAGS) -o msdscript $(OBJS)

test_msdscript: randomTests.o exec.o
	$(CXX) $(CXXFLAGS) -O2 -o test_msdscript randomTests.o exec.o

.PHONY: test
test: msdscript
	./msdscript --test

main.o: main.cpp cmdline.h Expr.h
	$(CXX) $(CXXFLAGS) -O2 -c main.cpp

cmdline.o: cmdline.cpp cmdline.h catch.h Expr.h Val.h parse.h Env.h
	$(CXX) $(CXXFLAGS) -O2 -c cmdline.cpp

Expr.o: Expr.cpp Expr.h catch.h Val.h
	$(CXX) $(CXXFLAGS) -O2 -c Expr.cpp

NumExpr.o: NumExpr.cpp NumExpr.h catch.h VarExpr.h NumVal.h Env.h
	$(CXX) $(CXXFLAGS) -O2 -c NumExpr.cpp

AddExpr.o: AddExpr.cpp AddExpr.h MultExpr.h NumExpr.h VarExpr.h catch.h NumVal.h Env.h
	$(CXX) $(CXXFLAGS) -O2 -c AddExpr.cpp

MultExpr.o: MultExpr.cpp MultExpr.h AddExpr.h NumExpr.h VarExpr.h catch.h NumVal.h Env.h
	$(CXX) $(CXXFLAGS) -O2 -c MultExpr.cpp

VarExpr.o: VarExpr.cpp VarExpr.h AddExpr.h NumExpr.h MultExpr.h catch.h Env.h
	$(CXX) $(CXXFLAGS) -O2 -c VarExpr.cpp

LetExpr.o: LetExpr.cpp LetExpr.h AddExpr.h NumExpr.h MultExpr.h VarExpr.h catch.h NumVal.h Env.h ExtendedEnv.h
	$(CXX) $(CXXFLAGS) -O2 -c LetExpr.cpp

BoolExpr.o: BoolExpr.cpp BoolExpr.h BoolVal.h NumExpr.h catch.h Env.h
	$(CXX) $(CXXFLAGS) -O2 -c BoolExpr.cpp

EqExpr.o: EqExpr.cpp EqExpr.h BoolVal.h AddExpr.h NumExpr.h MultExpr.h VarExpr.h catch.h BoolExpr.h Env.h
	$(CXX) $(CXXFLAGS) -O2 -c EqExpr.cpp

IfExpr.o: IfExpr.cpp IfExpr.h AddExpr.h BoolExpr.h EqExpr.h LetExpr.h MultExpr.h NumExpr.h VarExpr.h catch.h Val.h BoolVal.h NumVal.h Env.h
	$(CXX) $(CXXFLAGS) -O2 -c IfExpr.cpp

FunExpr.o: FunExpr.cpp FunExpr.h AddExpr.h CallExpr.h MultExpr.h NumExpr.h VarExpr.h catch.h FunVal.h Env.h
	$(CXX) $(CXXFLAGS) -O2 -c FunExpr.cpp

CallExpr.o: CallExpr.cpp CallExpr.h AddExpr.h BoolExpr.h MultExpr.h NumExpr.h VarExpr.h catch.h Val.h BoolVal.h NumVal.h FunVal.h Env.h
	$(CXX) $(CXXFLAGS) -O2 -c CallExpr.cpp

Val.o: Val.cpp Val.h
	$(CXX) $(CXXFLAGS) -O2 -c Val.cpp

NumVal.o: NumVal.cpp NumVal.h BoolVal.h NumExpr.h catch.h
	$(CXX) $(CXXFLAGS) -O2 -c NumVal.cpp

BoolVal.o: BoolVal.cpp BoolVal.h BoolExpr.h NumVal.h catch.h
	$(CXX) $(CXXFLAGS) -O2 -c BoolVal.cpp

FunVal.o: FunVal.cpp FunVal.h NumVal.h BoolVal.h catch.h VarExpr.h AddExpr.h NumExpr.h FunExpr.h BoolExpr.h ExtendedEnv.h
	$(CXX) $(CXXFLAGS) -O2 -c FunVal.cpp

Env.o: Env.cpp Env.h EmptyEnv.h catch.h
	$(CXX) $(CXXFLAGS) -O2 -c Env.cpp

EmptyEnv.o: EmptyEnv.cpp EmptyEnv.h ExtendedEnv.h NumVal.h catch.h
	$(CXX) $(CXXFLAGS) -O2 -c EmptyEnv.cpp

ExtendedEnv.o: ExtendedEnv.cpp ExtendedEnv.h EmptyEnv.h Val.h NumVal.h catch.h
	$(CXX) $(CXXFLAGS) -O2 -c ExtendedEnv.cpp

parse.o: parse.cpp parse.h Expr.h AddExpr.h BoolExpr.h CallExpr.h EqExpr.h FunExpr.h IfExpr.h LetExpr.h MultExpr.h NumExpr.h VarExpr.h catch.h NumVal.h
	$(CXX) $(CXXFLAGS) -O2 -c parse.cpp

exec.o: exec.cpp exec.h
	$(CXX) $(CXXFLAGS) -O2 -c exec.cpp

randomTests.o: randomTests.cpp exec.h
	$(CXX) $(CXXFLAGS) -O2 -c randomTests.cpp

clean:
	rm msdscript $(OBJS)