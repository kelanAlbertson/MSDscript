//
// Created by Kelan Albertson on 1/11/22.
//

#define CATCH_CONFIG_RUNNER
#include "catch.h"

#include <string>
#include <iostream>

#include "cmdline.h"
#include "Expr.h"
#include "Val.h"
#include "Env.h"
#include "parse.h"
#include "Step.h"

int use_arguments (int argc, char* argv[]) {
    bool  alreadyTested = false;
    if (argc == 1) {
        std::cout << "Too few arguments! Use '--help' for help\n";
    }
    else if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg == "--help") {
                std::cout << "Possible arguments:" << std::endl;
                std::cout << "  '--help' displays all possible arguments" << std::endl;
                std::cout << "  '--test' runs tests" << std::endl;
                std::cout << "  '--interp' accepts a single expression and prints the interpreted value of it using the stack's continuations" << std::endl;
                std::cout << "  '--step' accepts a single expression and prints the interpreted value of it using our continuations" << std::endl;
                std::cout << "  '--print' accepts a single expression and prints it with basic formatting" << std::endl;
                std::cout << "  '--pretty-print' accepts a single expression and prints it with better formatting" << std::endl;
                exit(0);
            }
            else if (arg == "--test") {
                if (alreadyTested) {
                    std::cerr << "Error: '--test' argument can only be used once" << std::endl;
                    exit(1);
                }
                else {
                    if (Catch::Session().run(1, argv) != 0) {
                        exit(0);
                    }
                    alreadyTested = true;
                }
            }
            else if (arg == "--interp") {
                PTR(Val) interpreted = parse(std::cin)->interp(Env::empty);
                std::cout << interpreted->to_string() << "\n";
                exit(0);
            }
            else if (arg == "--step") {
                PTR(Expr) parsed = parse(std::cin);
                PTR(Val) interpreted = Step::interp_by_steps(parsed);
                std::cout << interpreted->to_string() << "\n";
                exit(0);
            }
            else if (arg == "--print") {
                parse(std::cin)->print(std::cout);
                std::cout << "\n";
                exit(0);
            }
            else if (arg == "--pretty-print") {
                parse(std::cin)->pretty_print(std::cout);
                std::cout << "\n";
                exit(0);
            }
            else {
                std::cerr << "Error: invalid argument, use '--help' to see all acceptable arguments" << std::endl;
                exit(1);
            }
        }
    }
    return 0;
}
