//
// Created by Kelan Albertson on 2/7/22.
//

#pragma once

#include "pointer.h"

class Expr;
class LetExpr;
class NumExpr;
class VarExpr;
class IfExpr;
class FunExpr;

//parses an Expr from the input provided through the istream
PTR(Expr) parse(std::istream &in);
//parses an Expr from the string passed in
//just for testing purposes
PTR(Expr) parse_string(std::string s);
//parses an Expr
static PTR(Expr) parse_expr(std::istream &in);
//parses a comparg
static PTR(Expr) parse_comparg(std::istream &in);
//parses an added
static PTR(Expr) parse_addend(std::istream &in);
//parses a multicand
static PTR(Expr) parse_multicand(std::istream &in);
//parses an inner
static PTR(Expr) parse_inner(std::istream & in);
//parses a NumExpr
static PTR(NumExpr) parse_num(std::istream &in);
//parses a VarExpr
static PTR(VarExpr) parse_var(std::istream &in);
//parses a LetExpr
static PTR(LetExpr) parse_let(std::istream &in);
//parses an IfExpr
static PTR(IfExpr) parse_if(std::istream &in);
//parses a FunExpr
static PTR(FunExpr) parse_fun(std::istream &in);
//skips any whitespace in the istream
static void skip_whitespace(std::istream &in);
//consumes the next character in the istream as long as it matches the character that is expected, if not it throws an error
static void consume(std::istream &in, int expect);
//checks that the next characters of the stream match the expected string and consumes them if they do
static void consume_string(std::istream &in, std::string expect);
//parses a string of chars up until the next non-alpha char
static std::string parse_keyword(std::istream &in);
