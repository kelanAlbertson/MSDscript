//
// Created by Kelan Albertson on 1/11/22.
//

#define CATCH_CONFIG_RUNNER
#include "catch.h"

#include <string>
#include <iostream>

#include "cmdline.h"
#include "expr.h"
#include "parse.h"

int use_arguments (int argc, char *argv[]) {
    bool  alreadyTested = false;
    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg == "--help") {
                std::cout << "Possible arguments:" << std::endl;
                std::cout << "  '--help' displays all possible arguments" << std::endl;
                std::cout << "  '--test' runs tests" << std::endl;
                std::cout << "  '--interp' accepts a single expression and prints the interp value of it" << std::endl;
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
                        exit(1);
                    }
                    alreadyTested = true;
                }
            }
            else if (arg == "--interp") {
                int interpreted = parse(std::cin)->interp();
                std::cout << interpreted << "\n";
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
                std::cerr << "Error: invalid argument, can only handle '--help' or '--test'" << std::endl;
                exit(1);
            }
        }
    }
    return 0;
}
