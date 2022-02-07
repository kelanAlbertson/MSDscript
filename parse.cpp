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

//TODO list:
//  fix the -   5 returning 0 bug
//  implement mults, vars, lets

//static Expr *parse_expr(std::istream &in);
//static Expr *parse_addend(std::istream &in);
//static Expr *parse_num(std::istream &in);
//static void skip_whitespace(std::istream &in);
//static void consume(std::istream &in, int expect);

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
    else {
        consume(in, c);
        throw std::runtime_error("invalid input");
    }
}

static Expr *parse_num(std::istream &in) {
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

TEST_CASE("parse numbers") {
    CHECK( parse_string("1")->equals(new Num(1)) );
    CHECK( parse_string("1234")->equals(new Num(1234)) );
    CHECK( parse_string("-3")->equals(new Num(-3)) );
    CHECK( parse_string("  \n 5  ")->equals(new Num(5)) );
    CHECK( parse_string("(1)")->equals(new Num(1)) );
    CHECK( parse_string("(((1)))")->equals(new Num(1)) );
}


/* Some ideas for parse tests, probably not enough. Also, you may need
   to adjust these tests, and there may even be mistakes here. */
TEST_CASE("parse") {
    CHECK_THROWS_WITH( parse_string("()"), "bad input" );

    CHECK( parse_string("(1)")->equals(new Num(1)) );
    CHECK( parse_string("(((1)))")->equals(new Num(1)) );

    CHECK_THROWS_WITH( parse_string("(1"), "bad input" );

    CHECK( parse_string("1")->equals(new Num(1)) );
    CHECK( parse_string("10")->equals(new Num(10)) );
    CHECK( parse_string("-3")->equals(new Num(-3)) );
    CHECK( parse_string("  \n 5  ")->equals(new Num(5)) );

    CHECK_THROWS_WITH( parse_string("-"), "invalid input" );
    // This was some temporary debugging code:
    //  std::istringstream in("-");
    //  parse_num(in)->print(std::cout); std::cout << "\n";

    CHECK_THROWS_WITH( parse_string(" -   5  "), "invalid input" );

    CHECK( parse_string("x")->equals(new Var("x")) );
    CHECK( parse_string("xyz")->equals(new Var("xyz")) );
    CHECK( parse_string("xYz")->equals(new Var("xYz")) );
    CHECK_THROWS_WITH( parse_string("x_z"), "invalid input" );

    CHECK( parse_string("x + y")->equals(new Add(new Var("x"), new Var("y"))) );
    CHECK( parse_string("x * y")->equals(new Mult(new Var("x"), new Var("y"))) );
    CHECK( parse_string("z * x + y")
                   ->equals(new Add(new Mult(new Var("z"), new Var("x")),
                                    new Var("y"))) );

    CHECK( parse_string("z * (x + y)")
                   ->equals(new Mult(new Var("z"),
                                     new Add(new Var("x"), new Var("y"))) ));
}

