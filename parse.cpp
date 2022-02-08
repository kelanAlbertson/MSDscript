//
// Created by Kelan Albertson on 2/7/22.
//

#include <sstream>
#include "parse.h"
#include "add.h"
#include "expr.h"
#include "num.h"
#include "catch.h"
#include "var.h"
#include "mult.h"
#include "_let.h"

static Expr *parse(std::istream &in) {
    Expr *e = parse_expr(in);

    skip_whitespace(in);
    if (!in.eof()) {
        throw std::runtime_error("unexpected input after expression");
    }

    return e;
}

static Expr *parse_string(std::string s) {
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
        return new Add(e, rhs);
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
        return new Mult(e, rhs);
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

static Num *parse_num(std::istream &in) {
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

    return new Num(n);
}

static Var *parse_var(std::istream &in) {
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

    return new Var(s);
}

static _let *parse_let(std::istream &in) {
    std::string letText = "_let";
    for (int i = 0; i < 4; ++i) {
        int c = in.peek();
        if (c == letText[i]) {
            consume(in, letText[i]);
        }
        else {
            throw std::runtime_error("invalid input after underscore");
        }
    }

    skip_whitespace(in);

    Var *lhs = parse_var(in);

    skip_whitespace(in);

    int c = in.peek();
    if (c == '=') {
        consume(in, '=');
    }
    else {
        throw std::runtime_error("invalid input in _let expression");
    }

    Expr *rhs = parse_expr(in);

    skip_whitespace(in);

    std::string inText = "_in";
    for (int i = 0; i < 3; ++i) {
        int c = in.peek();
        if (c == inText[i]) {
            consume(in, inText[i]);
        }
        else {
            throw std::runtime_error("invalid input in _let expression");
        }
    }

    Expr *body = parse_expr(in);

    return new _let(lhs, rhs, body);
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

TEST_CASE("parse errors") {
    CHECK_THROWS_WITH( parse_string(""), "invalid input");
    CHECK_THROWS_WITH( parse_string("()"), "invalid input" );
    CHECK_THROWS_WITH( parse_string("(1"), "missing close parenthesis" );
    CHECK_THROWS_WITH( parse_string("-"), "invalid input" );
    CHECK_THROWS_WITH( parse_string(" -   5  "), "invalid input" );
    CHECK_THROWS_WITH(parse_string("0 + "), "invalid input");
    CHECK_THROWS_WITH(parse_string("0        ++"), "invalid input");
    CHECK_THROWS_WITH(parse_string("*t"), "invalid input");
    CHECK_THROWS_WITH( parse_string("x_z"), "unexpected input after expression" );
    CHECK_THROWS_WITH( parse_string("x Y"), "unexpected input after expression" );
    CHECK_THROWS_WITH( parse_string("_leet x = 5 _in 1"), "invalid input after underscore");
    CHECK_THROWS_WITH( parse_string("_let x 5 _in 1"), "invalid input in _let expression");
    CHECK_THROWS_WITH( parse_string("_let x = 5 _on 1"), "invalid input in _let expression");
    //test consume with expect mismatch
}

TEST_CASE("parse Nums") {
    CHECK( parse_string("1")->equals(new Num(1)) );
    CHECK( parse_string("1234")->equals(new Num(1234)) );
    CHECK( parse_string("-3")->equals(new Num(-3)) );
    CHECK( parse_string("  \n 5  ")->equals(new Num(5)) );
    CHECK( parse_string("(1)")->equals(new Num(1)) );
    CHECK( parse_string("(((1)))")->equals(new Num(1)) );
}

TEST_CASE("parse Add") {
    CHECK( parse_string("1 + 2")->equals(new Add(new Num(1), new Num(2))) );
    CHECK( parse_string("\tx+y")->equals(new Add(new Var("x"), new Var("y"))) );
    CHECK( parse_string("(-99 + a) + (2 + 2)")
            ->equals(new Add(new Add(new Num(-99), new Var("a")), new Add(new Num(2), new Num(2)))));
}

TEST_CASE("parse Mult") {
    CHECK( parse_string("1     *         2")->equals(new Mult(new Num(1), new Num(2))) );
    CHECK( parse_string("x * y")->equals(new Mult(new Var("x"), new Var("y"))) );
    CHECK( parse_string("(0*12345)\n*a")->equals(new Mult(new Mult(new Num(0), new Num(12345)), new Var("a"))) );
}

TEST_CASE("parse Var") {
    CHECK( parse_string("x")->equals(new Var("x")) );
    CHECK( parse_string("xyz")->equals(new Var("xyz")) );
    CHECK( parse_string("xYZ")->equals(new Var("xYZ")) );
}

TEST_CASE("parse _let") {
    CHECK( parse_string("  _let  x  =  5  _in  x  +  1")->equals(new _let(new Var("x"), new Num(5), new Add(new Var("x"), new Num(1)))) );
    CHECK( parse_string("_letx=5_in(_let y = 3_iny+2)+x")
            ->equals(new _let(new Var("x"), new Num(5), new Add(new _let(new Var("y"), new Num(3), new Add(new Var("y"), new Num(2))), new Var("x")))) );
}

TEST_CASE("parse combined") {
    CHECK( parse_string("z * x + y")
                   ->equals(new Add(new Mult(new Var("z"), new Var("x")),
                                    new Var("y"))) );

    CHECK( parse_string("z * (x + y)")
                   ->equals(new Mult(new Var("z"),
                                     new Add(new Var("x"), new Var("y"))) ));
    //add more to these
}


/* Some ideas for parse tests, probably not enough. Also, you may need
   to adjust these tests, and there may even be mistakes here. */
//TEST_CASE("parse") {
//    CHECK_THROWS_WITH( parse_string("()"), "bad input" );
//
//    CHECK( parse_string("(1)")->equals(new Num(1)) );
//    CHECK( parse_string("(((1)))")->equals(new Num(1)) );
//
//    CHECK_THROWS_WITH( parse_string("(1"), "bad input" );
//
//    CHECK( parse_string("1")->equals(new Num(1)) );
//    CHECK( parse_string("10")->equals(new Num(10)) );
//    CHECK( parse_string("-3")->equals(new Num(-3)) );
//    CHECK( parse_string("  \n 5  ")->equals(new Num(5)) );
//
//    CHECK_THROWS_WITH( parse_string("-"), "invalid input" );
//    // This was some temporary debugging code:
//    //  std::istringstream in("-");
//    //  parse_num(in)->print(std::cout); std::cout << "\n";
//
//    CHECK_THROWS_WITH( parse_string(" -   5  "), "invalid input" );
//
//    CHECK( parse_string("x")->equals(new Var("x")) );
//    CHECK( parse_string("xyz")->equals(new Var("xyz")) );
//    CHECK( parse_string("xYz")->equals(new Var("xYz")) );
//    CHECK_THROWS_WITH( parse_string("x_z"), "invalid input" );
//
//    CHECK( parse_string("x + y")->equals(new Add(new Var("x"), new Var("y"))) );
//    CHECK( parse_string("x * y")->equals(new Mult(new Var("x"), new Var("y"))) );
//    CHECK( parse_string("z * x + y")
//                   ->equals(new Add(new Mult(new Var("z"), new Var("x")),
//                                    new Var("y"))) );
//
//    CHECK( parse_string("z * (x + y)")
//                   ->equals(new Mult(new Var("z"),
//                                     new Add(new Var("x"), new Var("y"))) ));
//}

