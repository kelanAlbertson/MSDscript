//
// Created by Kelan Albertson on 2/16/22.
//


/*
 * TODO:
 *
 * Read these bullets before you start, but also go back and read all them again after you think you're done.
 X Create new files "Val.cpp" and "Val.h" (or whatever file names you prefer) for the new Val classes.
 X In "expr.h", use a class Val; declaration near the top so that the declaration of Expr::interp can refer to Val*. Do not use #include "Val.h", because the declaration of Expr classes should not need the implementation of Val classes.
 X In "expr.cpp", use #include "Val.h", because the implementation of Expr methods will need to use the implementation of Val classes and methods.
 X In "Val.h", similarly use class Expr; to allow Val::to_expr's return type to be Expr*. In "Val.cpp", use #include "expr.h" in addition to #include "Val.h".
 X You'll need to update your existing Expr::interp tests to use number-value objects instead of integers. Remember to change == for comparing integers to ->equals for comparing value objects.
 X If you're using Xcode, don't forget to add "Val.cpp" to your test target as well as your main target.
 X To full test a method like NumVal::equals, you'll need a pointer of type Val* that is not a pointer to a NumVal object — but NumVal is the only instantiable Val class at first! To work around that problem, use nullptr in tests for now (even though that's usually a bad idea), since nullptr can have type Val* and is not a pointer to a NumVal object.
 X To deal with adding values in AddExpr::interp, see the slides for a recommendation on adding a method Val::add_to.
 * To be able to print a Val* that is the result of Expr::interp, you'll need something like a Val::to_string method. If you send a Val* object directly to std::cout <<, it will just print as the hexadecimal address where the object is allocated, which is not useful.
 * Don't forget to update "Makefile".
 */

#include "Val.h"
#include "Expr.h"
