//
// Created by Kelan Albertson on 2/7/22.
//

#pragma once

class Expr;

//parses an Expr from the input provided through the istream
static Expr *parse(std::istream &in);

//parses an Expr from the string passed in
//just for testing purposes
static Expr *parse_string(std::string s);

//parses an Expr
static Expr *parse_expr(std::istream &in);

//parses an added
static Expr *parse_addend(std::istream &in);

//parses a Num
static Expr *parse_num(std::istream &in);

//skips any whitespace in the istream
static void skip_whitespace(std::istream &in);

//consumes the next character in the istream as long as it matches the character that is expected, if not it throws an error
static void consume(std::istream &in, int expect);
