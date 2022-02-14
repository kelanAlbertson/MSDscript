//
// Created by Kelan Albertson on 2/7/22.
//

#pragma once

class Expr;
class _let;
class Num;
class Var;

//parses an Expr from the input provided through the istream
Expr *parse(std::istream &in);
//parses an Expr from the string passed in
//just for testing purposes
Expr *parse_string(std::string s);
//parses an Expr
static Expr *parse_expr(std::istream &in);
//parses an added
static Expr *parse_addend(std::istream &in);
//parses a multicand
static Expr *parse_multicand(std::istream &in);
//parses a Num
static Num *parse_num(std::istream &in);
//parses a Var
static Var *parse_var(std::istream &in);
//parses a _let
static _let *parse_let(std::istream &in);
//skips any whitespace in the istream
static void skip_whitespace(std::istream &in);
//consumes the next character in the istream as long as it matches the character that is expected, if not it throws an error
static void consume(std::istream &in, int expect);
//checks that the next characters of the stream match the expected string and consumes them if they do
static void checkString(std::istream &in, std::string expect);
