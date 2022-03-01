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

//TODO UPDATE

Expr *parse(std::istream &in) {
    Expr *e = parse_expr(in);

    skip_whitespace(in);
    if (!in.eof()) {
        throw std::runtime_error("unexpected input after expression");
    }

    return e;
}

Expr *parse_string(std::string s) {
    std::stringstream ss(s);
    return parse(ss);
}

static Expr *parse_expr(std::istream &in) {
    Expr *e;

    e = parse_comparg(in);

    skip_whitespace(in);

    int c = in.peek();
    if (c == '=') {
        checkString(in, "==");
        Expr *rhs = parse_expr(in);
        return new EqExpr(e, rhs);
    }
    else {
        return e;
    }
}

static Expr *parse_comparg(std::istream &in) {
    Expr *e;

    e = parse_addend(in);

    skip_whitespace(in);

    int c = in.peek();
    if (c == '+') {
        consume(in, '+');
        Expr *rhs = parse_comparg(in);
        return new AddExpr(e, rhs);
    }
    else {
        return e;
    }
}

static Expr *parse_addend(std::istream &in) {
    Expr *e;

    e = parse_multicand(in);

    skip_whitespace(in);

    int c = in.peek();
    if (c == '*') {
        consume(in, '*');
        Expr *rhs = parse_addend(in);
        return new MultExpr(e, rhs);
    }
    else {
        return e;
    }
}

static Expr *parse_multicand(std::istream &in) {
    skip_whitespace(in);

    int c = in.peek();
    if (c == '-' || isdigit(c)) {
        return parse_num(in);
    }
    else if (c == '(') {
        consume(in, '(');
        Expr *e = parse_expr(in);
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
        c = in.peek();
        if (c == 'l') {
            return parse_let(in);
        }
        else if (c == 't') {
            checkString(in, "true");
            return new BoolExpr(true);
        }
        else if (c == 'f') {
            checkString(in, "false");
            return new BoolExpr(false);
        }
        else if (c == 'i') {
            return parse_if(in);
        }
        else {
            consume(in, c);
            throw std::runtime_error("invalid input");
        }
    }
    else {
        consume(in, c);
        throw std::runtime_error("invalid input");
    }
}

static NumExpr *parse_num(std::istream &in) {
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
            n = 10*n + (c -'0');
        }
        else {
            break;
        }
    }

    if (negative) {
        n = -n;
    }

    return new NumExpr(n);
}

static VarExpr *parse_var(std::istream &in) {
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

    return new VarExpr(s);
}

static LetExpr *parse_let(std::istream &in) {
    checkString(in, "let"); // the underscore was already consumed

    skip_whitespace(in);

    VarExpr *lhs = parse_var(in);

    skip_whitespace(in);

    int c = in.peek();
    if (c == '=') {
        consume(in, '=');
    }
    else {
        throw std::runtime_error("missing or invalid location of equals sign in _let expression");
    }

    Expr *rhs = parse_expr(in);

    skip_whitespace(in);

    checkString(in, "_in");

    Expr *body = parse_expr(in);

    return new LetExpr(lhs, rhs, body);
}

static IfExpr *parse_if(std::istream &in) {
    checkString(in, "if"); // the underscore was already consumed

    Expr *condition = parse_expr(in);

    skip_whitespace(in);

    checkString(in, "_then");

    Expr *then_statement = parse_expr(in);

    skip_whitespace(in);

    checkString(in, "_else");

    Expr *else_statement = parse_expr(in);

    return new IfExpr(condition, then_statement, else_statement);
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

static void checkString(std::istream &in, std::string expect) {
    for (int i = 0; i < expect.length(); ++i) {
        int c = in.peek();
        if (c == expect[i]) {
            consume(in, expect[i]);
        }
        else {
            throw std::runtime_error("checkString mismatch");
        }
    }
}

static std::string parse_keyword(std::istream &in) {
    std::string result;
    int c = in.peek();
    while (isalpha(c)) {
        result += std::to_string(c);
    }
    return result;
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
    CHECK_THROWS_WITH( parse_string("_leet x = 5 _in 1"), "checkString mismatch");
    CHECK_THROWS_WITH( parse_string("_let x 5 _in 1"), "missing or invalid location of equals sign in _let expression");
    CHECK_THROWS_WITH( parse_string("_let x = 5 _on 1"), "checkString mismatch");
    std::stringstream test("test");
    CHECK_THROWS_WITH(consume(test, 'x'), "consume mismatch");
}

TEST_CASE("parse NumExpr") {
    CHECK( parse_string("1")->equals(new NumExpr(1)) );
    CHECK( parse_string("1234")->equals(new NumExpr(1234)) );
    CHECK( parse_string("-3")->equals(new NumExpr(-3)) );
    CHECK( parse_string("  \n 5  ")->equals(new NumExpr(5)) );
    CHECK( parse_string("(1)")->equals(new NumExpr(1)) );
    CHECK( parse_string("(((1)))")->equals(new NumExpr(1)) );
}

TEST_CASE("parse AddExpr") {
    CHECK( parse_string("1 + 2")->equals(new AddExpr(new NumExpr(1), new NumExpr(2))) );
    CHECK( parse_string("\tx+y")->equals(new AddExpr(new VarExpr("x"), new VarExpr("y"))) );
    CHECK( parse_string("(-99 + a) + (2 + 2)")
            ->equals(new AddExpr(new AddExpr(new NumExpr(-99), new VarExpr("a")), new AddExpr(new NumExpr(2), new NumExpr(2)))));
}

TEST_CASE("parse MultExpr") {
    CHECK( parse_string("1     *         2")->equals(new MultExpr(new NumExpr(1), new NumExpr(2))) );
    CHECK( parse_string("x * y")->equals(new MultExpr(new VarExpr("x"), new VarExpr("y"))) );
    CHECK( parse_string("(0*12345)\n*a")->equals(new MultExpr(new MultExpr(new NumExpr(0), new NumExpr(12345)), new VarExpr("a"))) );
    CHECK( parse_string("z * x + y")->equals(new AddExpr(new MultExpr(new VarExpr("z"), new VarExpr("x")), new VarExpr("y"))) );
    CHECK( parse_string("z * (x + y)")->equals(new MultExpr(new VarExpr("z"), new AddExpr(new VarExpr("x"), new VarExpr("y"))) ));
}

TEST_CASE("parse VarExpr") {
    CHECK( parse_string("x")->equals(new VarExpr("x")) );
    CHECK( parse_string("xyz")->equals(new VarExpr("xyz")) );
    CHECK( parse_string("xYZ")->equals(new VarExpr("xYZ")) );
}

TEST_CASE("parse LetExpr") {
    CHECK( parse_string("  _let  x  =  5  _in  x  +  1")->equals(new LetExpr(new VarExpr("x"), new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1)))) );
    CHECK( parse_string("_letx=5_in(_let y = 3_iny+2)+x")
            ->equals(new LetExpr(new VarExpr("x"), new NumExpr(5), new AddExpr(new LetExpr(new VarExpr("y"), new NumExpr(3), new AddExpr(new VarExpr("y"), new NumExpr(2))), new VarExpr("x")))) );
}

TEST_CASE("parse BoolExpr") {
    CHECK( parse_string("_true")->equals(new BoolExpr(true)) );
    CHECK( parse_string("\n\t_false ")->equals(new BoolExpr(false)) );
}

TEST_CASE("parse EqExpr") {
    CHECK( parse_string("1==1")->equals(new EqExpr(new NumExpr(1), new NumExpr(1))));
    CHECK( parse_string("    _true ==    1")->equals(new EqExpr(new BoolExpr(true), new NumExpr(1))));
    CHECK( parse_string("7 +\n x==_false+0")
            ->equals(new EqExpr(new AddExpr(new NumExpr(7), new VarExpr("x")), new AddExpr(new BoolExpr(false), new NumExpr(0)))) );
}

TEST_CASE("parse IfExpr") {
    CHECK(parse_string("_if 4 + 1\n_then 2\n_else 3")
            ->equals(new IfExpr(new AddExpr(new NumExpr(4), new NumExpr(1)), new NumExpr(2), new NumExpr(3))) );
    CHECK(parse_string("\t_if\t_true\t_then\t_true\t_else\t_false")
            ->equals(new IfExpr(new BoolExpr(true), new BoolExpr(true), new BoolExpr(false))) );
    CHECK(parse_string("(_if   (_if  (1 ==1) _then       _true _else _false) _then    10            _else  20)")
            ->equals((new IfExpr(new IfExpr(new EqExpr(new NumExpr(1), new NumExpr(1)), new BoolExpr(true), new BoolExpr(false)), new NumExpr(10), new NumExpr(20)))) );
    CHECK(parse_string("_if _false _then (0+0) _else (y==1)")
            ->equals(new IfExpr(new BoolExpr(false), new AddExpr(new NumExpr(0), new NumExpr(0)), new EqExpr(new VarExpr("y"), new NumExpr(1)))));
}