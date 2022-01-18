//
// Created by Kelan Albertson on 1/11/22.
//

#define CATCH_CONFIG_RUNNER
#include "catch.h"

#include <string>
#include <iostream>

#include "cmdline.h"

int use_arguments (int argc, char *argv[]) {
    bool  alreadyTested = false;
    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg == "--help") {
                std::cout << "Possible arguments:" << std::endl << "  '--help' displays all possible arguments" <<
                std::endl << "  '--test' displays the test results" << std::endl;
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
            else {
                std::cerr << "Error: invalid argument, can only handle '--help' or '--test'" << std::endl;
                exit(1);
            }
        }
    }
    return 0;
}
