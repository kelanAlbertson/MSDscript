//
// Created by Kelan Albertson on 1/11/22.
//

#include "cmdline.h"
#include "expr.h"
#include <iostream>

int main(int argc, char *argv[]) {

    try {
        use_arguments(argc, argv);
        return 0;
    }
    catch (std::runtime_error exn) {
        std::cerr << exn.what() << "\n";
        return 1;
    }
}
