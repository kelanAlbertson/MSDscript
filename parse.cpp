//
// Created by Kelan Albertson on 2/7/22.
//

#include <sstream>
#include <iostream>
#include "parse.h"
#include "AddExpr.h"
#include "Expr.h"
#include "NumExpr.h"
#include "catch.h"
#include "VarExpr.h"
#include "MultExpr.h"
#include "LetExpr.h"
#include "EqExpr.h"
#include "BoolExpr.h"
#include "IfExpr.h"
#include "FunExpr.h"
#include "CallExpr.h"
#include "NumVal.h"
#include "Env.h"

PTR(Expr) parse(std::istream &in) {
    PTR(Expr) e = parse_expr(in);

    skip_whitespace(in);
    if (!in.eof()) {
        throw std::runtime_error("unexpected input after expression");
    }

    return e;
}

PTR(Expr) parse_string(std::string s) {
    std::stringstream ss(s);
    return parse(ss);
}

static PTR(Expr) parse_expr(std::istream &in) {
    PTR(Expr) e = parse_comparg(in);

    skip_whitespace(in);

    if (in.peek() == '=') {
        consume_string(in, "==");
        PTR(Expr) rhs = parse_expr(in);
        return NEW(EqExpr)(e, rhs);
    }
    else {
        return e;
    }
}

static PTR(Expr) parse_comparg(std::istream &in) {
    PTR(Expr) e = parse_addend(in);

    skip_whitespace(in);

    if (in.peek() == '+') {
        consume(in, '+');
        PTR(Expr) rhs = parse_comparg(in);
        return NEW(AddExpr)(e, rhs);
    }
    else {
        return e;
    }
}

static PTR(Expr) parse_addend(std::istream &in) {
    PTR(Expr) e = parse_multicand(in);

    skip_whitespace(in);

    if (in.peek() == '*') {
        consume(in, '*');
        PTR(Expr) rhs = parse_addend(in);
        return NEW(MultExpr)(e, rhs);
    }
    else {
        return e;
    }
}

static PTR(Expr) parse_multicand(std::istream &in) {
    PTR(Expr) e = parse_inner(in);

    skip_whitespace(in);

    while (in.peek() == '(') {
        consume(in, '(');
        PTR(Expr) arg = parse_expr(in);
        skip_whitespace(in);
        if (in.get() != ')') {
            throw std::runtime_error("missing close parenthesis");
        }
        e = NEW(CallExpr)(e, arg);
    }
    return e;
}

static PTR(Expr) parse_inner(std::istream &in) {
    skip_whitespace(in);

    int c = in.peek();
    if (c == '-' || isdigit(c)) {
        return parse_num(in);
    }
    else if (c == '(') {
        consume(in, '(');
        PTR(Expr) e = parse_expr(in);
        skip_whitespace(in);
        c = in.get();
        if (c != ')') {
            throw std::runtime_error("missing close parenthesis");
        }
        return e;
    }
    else if (isalpha(c)) {
        return parse_var(in);
    }
    else if (c == '_') {
        consume(in, '_');
        std::string keyword = parse_keyword(in);
        if (keyword == "let") {
            return parse_let(in);
        }
        else if (keyword == "true") {
            return NEW(BoolExpr)(true);
        }
        else if (keyword == "false") {
            return NEW(BoolExpr)(false);
        }
        else if (keyword == "if") {
            return parse_if(in);
        }
        else if(keyword == "fun") {
            return parse_fun(in);
        }
        else {
            throw std::runtime_error("invalid input");
        }
    }
    else {
        consume(in, c);
        throw std::runtime_error("invalid input");
    }
}

static PTR(NumExpr) parse_num(std::istream &in) {
    int n = 0;
    bool negative = false;

    if (in.peek() == '-') {
        negative = true;
        consume(in, '-');
        if (!isdigit(in.peek())) {
            throw std::runtime_error("invalid input");
        }
    }

    while (1) {
        int c = in.peek();
        if (isdigit(c)) {
            consume(in, c);
            if (n > (INT_MAX - (c - '0'))/10) {
                throw std::runtime_error("parse_num caused int overflow");
            }
            else {
                n = 10*n + (c - '0');
            }
        }
        else {
            break;
        }
    }

    if (negative) {
        n = -n;
    }

    return NEW(NumExpr)(n);
}

static PTR(VarExpr) parse_var(std::istream &in) {
    std::string s = "";

    while (1) {
        int c = in.peek();
        if (isalpha(c)) {
            consume(in, c);
            s += c;
        }
        else {
            break;
        }
    }

    return NEW(VarExpr)(s);
}

static PTR(LetExpr) parse_let(std::istream &in) {
    skip_whitespace(in);

    PTR(VarExpr) lhs = parse_var(in);

    skip_whitespace(in);

    if (in.peek() == '=') {
        consume(in, '=');
    }
    else {
        throw std::runtime_error("missing or invalid location of equals sign in _let expression");
    }

    PTR(Expr) rhs = parse_expr(in);

    skip_whitespace(in);

    if (parse_keyword(in) != "_in") {
        throw std::runtime_error("missing or invalid location of '_in' in _let expression");
    }

    PTR(Expr) body = parse_expr(in);

    return NEW(LetExpr)(lhs, rhs, body);
}

static PTR(IfExpr) parse_if(std::istream &in) {
    PTR(Expr) condition = parse_expr(in);

    skip_whitespace(in);

    if (parse_keyword(in) != "_then") {
        throw std::runtime_error("missing or invalid location of '_then' in _if expression");
    }

    PTR(Expr) then_statement = parse_expr(in);

    skip_whitespace(in);

    if (parse_keyword(in) != "_else") {
        throw std::runtime_error("missing or invalid location of '_else' in _if expression");
    }

    PTR(Expr) else_statement = parse_expr(in);

    return NEW(IfExpr)(condition, then_statement, else_statement);
}

static PTR(FunExpr) parse_fun(std::istream &in) {
    skip_whitespace(in);

    if (in.peek() == '(') {
        consume(in, '(');
    }
    else {
        throw std::runtime_error("missing or invalid location of '(' in _fun expression");
    }

    skip_whitespace(in);

    PTR(VarExpr) arg = parse_var(in);

    skip_whitespace(in);

    if (in.peek() == ')') {
        consume(in, ')');
    }
    else {
        throw std::runtime_error("missing or invalid location of ')' in _fun expression");
    }

    PTR(Expr) body = parse_expr(in);

    return NEW(FunExpr)(arg, body);
}

static void skip_whitespace(std::istream &in) {
    while(1) {
        int c = in.peek();
        if (!isspace(c)) {
            break;
        }
        consume(in, c);
    }
}

static void consume(std::istream &in, int expect) {
    int c = in.get();
    if (c != expect) {
        throw std::runtime_error("consume mismatch");
    }
}

static void consume_string(std::istream &in, std::string expect) {
    for (int i = 0; i < expect.length(); ++i) {
        if (in.peek() == expect[i]) {
            consume(in, expect[i]);
        }
        else {
            throw std::runtime_error("consume_string mismatch");
        }
    }
}

static std::string parse_keyword(std::istream &in) {
    std::string s = "";
    while (1) {
        int c = in.peek();
        if (isalpha(c) || c == '_') {
            consume(in, c);
            s += c;
        }
        else {
            break;
        }
    }
    return s;
}

TEST_CASE("parse errors") {
    CHECK_THROWS_WITH( parse_string(""), "invalid input");
    CHECK_THROWS_WITH( parse_string("()"), "invalid input" );
    CHECK_THROWS_WITH( parse_string("(1"), "missing close parenthesis" );
    CHECK_THROWS_WITH( parse_string("-"), "invalid input" );
    CHECK_THROWS_WITH( parse_string(" -   5  "), "invalid input" );
    CHECK_THROWS_WITH( parse_string("0 + "), "invalid input");
    CHECK_THROWS_WITH( parse_string("0        ++"), "invalid input");
    CHECK_THROWS_WITH( parse_string("*t"), "invalid input");
    CHECK_THROWS_WITH( parse_string("x_z"), "unexpected input after expression" );
    CHECK_THROWS_WITH( parse_string("x Y"), "unexpected input after expression" );
    CHECK_THROWS_WITH( parse_string("_leet x = 5 _in 1"), "invalid input");
    CHECK_THROWS_WITH( parse_string("_let x 5 _in 1"), "missing or invalid location of equals sign in _let expression");
    CHECK_THROWS_WITH( parse_string("_let x = 5 _on 1"), "missing or invalid location of '_in' in _let expression");
    CHECK_THROWS_WITH( parse_string("_if _true _else _true"), "missing or invalid location of '_then' in _if expression");
    CHECK_THROWS_WITH( parse_string("   _if x == 2 _then 2 _eeeeeeeelse 0"), "missing or invalid location of '_else' in _if expression");
    CHECK_THROWS_WITH( parse_string("(  _fun (x) x  ) (0"), "missing close parenthesis");
    CHECK_THROWS_WITH( parse_string("_fun y 1"), "missing or invalid location of '(' in _fun expression");
    CHECK_THROWS_WITH( parse_string("_fun (y 1"), "missing or invalid location of ')' in _fun expression");
    std::stringstream test("test");
    CHECK_THROWS_WITH(consume(test, 'x'), "consume mismatch");
    CHECK_THROWS_WITH(consume_string(test, "testing"), "consume_string mismatch");
}

TEST_CASE("parse NumExpr") {
    CHECK( parse_string("1")->equals(NEW(NumExpr)(1)) );
    CHECK( parse_string("1234")->equals(NEW(NumExpr)(1234)) );
    CHECK( parse_string("-3")->equals(NEW(NumExpr)(-3)) );
    CHECK( parse_string("  \n 5  ")->equals(NEW(NumExpr)(5)) );
    CHECK( parse_string("(1)")->equals(NEW(NumExpr)(1)) );
    CHECK( parse_string("(((1)))")->equals(NEW(NumExpr)(1)) );
}

TEST_CASE("parse AddExpr") {
    CHECK( parse_string("1 + 2")->equals(NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2))) );
    CHECK( parse_string("\tx+y")->equals(NEW(AddExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y"))) );
    CHECK( parse_string("(-99 + a) + (2 + 2)")
            ->equals(NEW(AddExpr)(NEW(AddExpr)(NEW(NumExpr)(-99), NEW(VarExpr)("a")), NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(2)))));
}

TEST_CASE("parse MultExpr") {
    CHECK( parse_string("1     *         2")->equals(NEW(MultExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2))) );
    CHECK( parse_string("x * y")->equals(NEW(MultExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y"))) );
    CHECK( parse_string("(0*12345)\n*a")->equals(NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(0), NEW(NumExpr)(12345)), NEW(VarExpr)("a"))) );
    CHECK( parse_string("z * x + y")->equals(NEW(AddExpr)(NEW(MultExpr)(NEW(VarExpr)("z"), NEW(VarExpr)("x")), NEW(VarExpr)("y"))) );
    CHECK( parse_string("z * (x + y)")->equals(NEW(MultExpr)(NEW(VarExpr)("z"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y"))) ));
}

TEST_CASE("parse VarExpr") {
    CHECK( parse_string("x")->equals(NEW(VarExpr)("x")) );
    CHECK( parse_string("xyz")->equals(NEW(VarExpr)("xyz")) );
    CHECK( parse_string("xYZ")->equals(NEW(VarExpr)("xYZ")) );
}

TEST_CASE("parse LetExpr") {
    CHECK( parse_string("  _let  x  =  5  _in  x  +  1")->equals(NEW(LetExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))) );
    CHECK( parse_string("_let x=5 _in (_let y = 3 _in y+2)+x")
            ->equals(NEW(LetExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5), NEW(AddExpr)(NEW(LetExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(3), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))), NEW(VarExpr)("x")))) );
}

TEST_CASE("parse BoolExpr") {
    CHECK( parse_string("_true")->equals(NEW(BoolExpr)(true)) );
    CHECK( parse_string("\n\t_false ")->equals(NEW(BoolExpr)(false)) );
}

TEST_CASE("parse EqExpr") {
    CHECK( parse_string("1==1")->equals(NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(1))));
    CHECK( parse_string("    _true ==    1")->equals(NEW(EqExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(1))));
    CHECK( parse_string("7 +\n x==_false+0")
            ->equals(NEW(EqExpr)(NEW(AddExpr)(NEW(NumExpr)(7), NEW(VarExpr)("x")), NEW(AddExpr)(NEW(BoolExpr)(false), NEW(NumExpr)(0)))) );
}

TEST_CASE("parse IfExpr") {
    CHECK(parse_string("_if 4 + 1\n_then 2\n_else 3")
            ->equals(NEW(IfExpr)(NEW(AddExpr)(NEW(NumExpr)(4), NEW(NumExpr)(1)), NEW(NumExpr)(2), NEW(NumExpr)(3))) );
    CHECK(parse_string("\t_if\t_true\t_then\t_true\t_else\t_false")
            ->equals(NEW(IfExpr)(NEW(BoolExpr)(true), NEW(BoolExpr)(true), NEW(BoolExpr)(false))) );
    CHECK(parse_string("(_if   (_if  (1 ==1) _then       _true _else _false) _then    10            _else  20)")
            ->equals((NEW(IfExpr)(NEW(IfExpr)(NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(1)), NEW(BoolExpr)(true), NEW(BoolExpr)(false)), NEW(NumExpr)(10), NEW(NumExpr)(20)))) );
    CHECK(parse_string("_if _false _then (0+0) _else (y==1)")
            ->equals(NEW(IfExpr)(NEW(BoolExpr)(false), NEW(AddExpr)(NEW(NumExpr)(0), NEW(NumExpr)(0)), NEW(EqExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(1)))));
}

TEST_CASE("parse FunExpr") {
    CHECK(parse_string("_fun (x) x+1")
            ->equals(NEW(FunExpr)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))));
    CHECK(parse_string("(_fun (x) (_fun (y) ((x*x)+(y*y))))")
            ->equals(NEW(FunExpr)(NEW(VarExpr)("x"), NEW(FunExpr)(NEW(VarExpr)("y"), NEW(AddExpr)(NEW(MultExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("x")), NEW(MultExpr)(NEW(VarExpr)("y"), NEW(VarExpr)("y")))))));
}

TEST_CASE("parse CallExpr") {
    CHECK(parse_string("(_fun (x) x)(2)")
            ->equals(NEW(CallExpr)(NEW(FunExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("x")), NEW(NumExpr)(2))));
    CHECK(parse_string("(_fun (x) (_fun (y) ((x*x)+(y*y)))(3))(2)")
            ->equals(NEW(CallExpr)(NEW(FunExpr)(NEW(VarExpr)("x"), NEW(CallExpr)(NEW(FunExpr)(NEW(VarExpr)("y"), NEW(AddExpr)(NEW(MultExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("x")), NEW(MultExpr)(NEW(VarExpr)("y"), NEW(VarExpr)("y")))), NEW(NumExpr)(3))),NEW(NumExpr)(2))));
}

TEST_CASE("Matthew's factorial test") {
    // this should do 10! which is equal to 3628800
    CHECK((parse_string("_let factrl = _fun (factrl)\n"
                 "                _fun (x)\n"
                 "                  _if x == 1\n"
                 "                  _then 1\n"
                 "                  _else x * factrl(factrl)(x + -1)\n"
                 "_in  factrl(factrl)(10)"))
                 ->interp(Env::empty)->equals(NEW(NumVal)(3628800)));
    PTR(Expr)e = parse_string("_let factrl = _fun (factrl)\n"
                  "                _fun (x)\n"
                  "                  _if x == 1\n"
                  "                  _then 1\n"
                  "                  _else x * factrl(factrl)(x + -1)\n"
                  "_in  factrl(factrl)(10)");

    CHECK((parse_string("_let factrl = _fun (factrl)\n"
                        "                _fun (x)\n"
                        "                  _if x == 1\n"
                        "                  _then 1\n"
                        "                  _else x * factrl(factrl)(x + -1)\n"
                        "_in  factrl(factrl)(10)"))
                  ->to_pretty_string() == "_let factrl = _fun (factrl)\n"
                                          "                _fun (x)\n"
                                          "                  _if x == 1\n"
                                          "                  _then 1\n"
                                          "                  _else x * factrl(factrl)(x + -1)\n"
                                          "_in  factrl(factrl)(10)");
}