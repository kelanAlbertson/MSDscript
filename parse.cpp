//
// Created by Kelan Albertson on 2/7/22.
//

#include <sstream>
#include "parse.h"
#include "AddExpr.h"
#include "Expr.h"
#include "NumExpr.h"
#include "catch.h"
#include "VarExpr.h"
#include "MultExpr.h"
#include "LetExpr.h"

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

    e = parse_addend(in);

    skip_whitespace(in);

    int c = in.peek();
    if (c == '+') {
        consume(in, '+');
        Expr *rhs = parse_expr(in);
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
        return parse_let(in);
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
    checkString(in, "_let");

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

TEST_CASE("parse Nums") {
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

TEST_CASE("parse combined") {
    CHECK( parse_string("z * x + y")
                   ->equals(new AddExpr(new MultExpr(new VarExpr("z"), new VarExpr("x")),
                                        new VarExpr("y"))) );

    CHECK( parse_string("z * (x + y)")
                   ->equals(new MultExpr(new VarExpr("z"),
                                         new AddExpr(new VarExpr("x"), new VarExpr("y"))) ));
    //add more to these
}


/* Some ideas for parse tests, probably not enough. Also, you may need
   to adjust these tests, and there may even be mistakes here. */
//TEST_CASE("parse") {
//    CHECK_THROWS_WITH( parse_string("()"), "bad input" );
//
//    CHECK( parse_string("(1)")->equals(new NumExpr(1)) );
//    CHECK( parse_string("(((1)))")->equals(new NumExpr(1)) );
//
//    CHECK_THROWS_WITH( parse_string("(1"), "bad input" );
//
//    CHECK( parse_string("1")->equals(new NumExpr(1)) );
//    CHECK( parse_string("10")->equals(new NumExpr(10)) );
//    CHECK( parse_string("-3")->equals(new NumExpr(-3)) );
//    CHECK( parse_string("  \n 5  ")->equals(new NumExpr(5)) );
//
//    CHECK_THROWS_WITH( parse_string("-"), "invalid input" );
//    // This was some temporary debugging code:
//    //  std::istringstream in("-");
//    //  parse_num(in)->print(std::cout); std::cout << "\n";
//
//    CHECK_THROWS_WITH( parse_string(" -   5  "), "invalid input" );
//
//    CHECK( parse_string("x")->equals(new VarExpr("x")) );
//    CHECK( parse_string("xyz")->equals(new VarExpr("xyz")) );
//    CHECK( parse_string("xYz")->equals(new VarExpr("xYz")) );
//    CHECK_THROWS_WITH( parse_string("x_z"), "invalid input" );
//
//    CHECK( parse_string("x + y")->equals(new AddExpr(new VarExpr("x"), new VarExpr("y"))) );
//    CHECK( parse_string("x * y")->equals(new MultExpr(new VarExpr("x"), new VarExpr("y"))) );
//    CHECK( parse_string("z * x + y")
//                   ->equals(new AddExpr(new MultExpr(new VarExpr("z"), new VarExpr("x")),
//                                    new VarExpr("y"))) );
//
//    CHECK( parse_string("z * (x + y)")
//                   ->equals(new MultExpr(new VarExpr("z"),
//                                     new AddExpr(new VarExpr("x"), new VarExpr("y"))) ));
//}

