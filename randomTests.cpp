//
// Created by Kelan Albertson on 2/14/22.
//

#include <string>
#include <iostream>
#include "exec.h"

std::string random_expr_string();

int main(int argc, char *argv[]) {
    srand(time(NULL));

    if (argc == 1 || argc > 3) {
        std::cerr << "Invalid number of arguments to test_msdscript\n";
        return 1;
    }

    const char *const interp1_argv[] = {argv[1], "--interp"};
    const char *const print1_argv[] = {argv[1], "--print"};
    const char *const pretty_print1_argv[] = {argv[1], "--pretty-print"};
    if (argc == 2) {
        for (int i = 0; i < 100; ++i) {
            std::string in = random_expr_string();
            std::cout << "Trying " << in << "\n";

            // test that interp, print, and pretty print return exit code 0
            ExecResult interp_result = exec_program(2, interp1_argv, in);
            if (interp_result.exit_code != 0) {
                throw std::runtime_error("interp did not return exit code 0");
            }
            ExecResult print_result = exec_program(2, print1_argv, in);
            if (interp_result.exit_code != 0) {
                throw std::runtime_error("print did not return exit code 0");
            }
            ExecResult pretty_print_result = exec_program(2, pretty_print1_argv, in);
            if (interp_result.exit_code != 0) {
                throw std::runtime_error("pretty_print did not return exit code 0");
            }

            // test that interp of print result returns the same result as the original interp result
            ExecResult interp_print_result = exec_program(2, interp1_argv, print_result.out);
            if (interp_print_result.out != interp_result.out) {
                throw std::runtime_error("interp of print result did not match original interp result");
            }

            // test that interp of pretty print result returns the same result as the original interp result
            ExecResult interp_pretty_print_result = exec_program(2, interp1_argv, pretty_print_result.out);
            if (interp_pretty_print_result.out != interp_result.out) {
                throw std::runtime_error("interp of pretty print result did not match original interp result");
            }

            // test that the pretty print of the pretty print result returns the same result as the original pretty print result
            ExecResult pretty_print_pretty_print_result = exec_program(2, pretty_print1_argv, pretty_print_result.out);
            if (pretty_print_pretty_print_result.out != pretty_print_result.out) {
                throw std::runtime_error("pretty print of the pretty print result did not match the original pretty print result");
            }
        }
    }
    else if (argc == 3) {
        const char *const interp2_argv[] = {argv[2], "--interp"};
        const char *const print2_argv[] = {argv[2], "--print"};
        const char *const pretty_print2_argv[] = {argv[2], "--pretty-print"};

        for (int i = 0; i < 100; ++i) {
            std::string in = random_expr_string();
            std::cout << "Trying " << in << "\n";

            // test that interp, print, and pretty print return the same result from both msdscript programs
            ExecResult interp1_result = exec_program(2, interp1_argv, in);
            ExecResult interp2_result = exec_program(2, interp2_argv, in);
            if (interp1_result.out != interp2_result.out) {
                throw std::runtime_error("interp results did not match");
            }
            ExecResult print1_result = exec_program(2, print1_argv, in);
            ExecResult print2_result = exec_program(2, print2_argv, in);
            if (print1_result.out != print2_result.out) {
                throw std::runtime_error("print results did not match");
            }
            ExecResult pretty_print1_result = exec_program(2, pretty_print1_argv, in);
            ExecResult pretty_print2_result = exec_program(2, pretty_print2_argv, in);
            if (pretty_print1_result.out != pretty_print2_result.out) {
                throw std::runtime_error("pretty_print results did not match");
            }
        }
    }


    return 0;
}

std::string random_var_name() {
    int rand_num = rand() % 10;
    if (rand_num < 6) {
        // 60% chance of naming the var "x"
        return "x";
    }
    else {
        // 40% chance of creating a completely random string
        std::string s = "";
        // want to force it to generate at least one char before terminating in order to avoid empty string
        // 50/50 chance of being uppercase or lowercase
        if ((rand_num % 2) == 0) {
            s += (char) ((rand() % 26) + 65);
        }
        else {
            s += (char) ((rand() % 26) + 97);
        }

        // once string is one character long then it could be terminated at any time
        rand_num = rand() % 100;
        // 34% chance of terminating the string and returning
        while (rand_num > 33) {
            // 33% chance of appending a random uppercase letter
            if ((rand_num % 2) == 0) {
                s += (char) ((rand() % 26) + 65);
            }
            // 33% chance of appending a random lowercase letter
            else {
                s += (char) ((rand() % 26) + 97);
            }

            rand_num = rand() % 100;
        }
        return s;
    }
}

std::string random_expr_without_unbound_vars() {
    int rand_num = rand() % 10;
    if (rand_num < 3) {
        // 30% chance of generating a number
        return std::to_string(rand());
    }
    else if (rand_num >= 3 && rand_num < 6) {
        // 30% chance of generating a string (variable)
        return random_var_name();
    }
    else if (rand_num == 6) {
        // 10% chance of generating a parenthesized expression
        // generates another random expression inside it
        return "(" + random_expr_string() + ")";
    }
    else if (rand_num == 7) {
        // 10% chance of generating an add
        // generates another two random expression inside it
        return random_expr_string() + "+" + random_expr_string();
    }
    else if (rand_num == 8) {
        // 10% chance of generating a mult
        // generates another two random expression inside it
        return random_expr_string() + "*" + random_expr_string();
    }
    else {
        // 10% chance of generating a _let
        // generates a random variable and two random expressions inside it
        return "_let " + random_var_name() + " = " + random_expr_string() + " _in " + random_expr_string();
    }
}


std::string random_expr_string() {
    int rand_num = rand() % 10;
    if (rand_num < 3) {
        // 30% chance of generating a number
        return std::to_string(rand());
    }
    else if (rand_num >= 3 && rand_num < 6) {
        // 30% chance of generating a string (variable)
        return random_var_name();
    }
    else if (rand_num == 6) {
        // 10% chance of generating a parenthesized expression
        // generates another random expression inside it
        return "(" + random_expr_string() + ")";
    }
    else if (rand_num == 7) {
        // 10% chance of generating an add
        // generates another two random expression inside it
        return random_expr_string() + "+" + random_expr_string();
    }
    else if (rand_num == 8) {
        // 10% chance of generating a mult
        // generates another two random expression inside it
        return random_expr_string() + "*" + random_expr_string();
    }
    else {
        // 10% chance of generating a _let
        // generates a random variable and two random expressions inside it
        return "_let " + random_var_name() + " = " + random_expr_string() + " _in " + random_expr_string();
    }
}


