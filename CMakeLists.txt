cmake_minimum_required(VERSION 3.20)
project(MSDscript)

set(CMAKE_CXX_STANDARD 14)

add_executable(MSDscript main.cpp cmdline.cpp expr.cpp num.cpp add.cpp mult.cpp var.cpp _let.cpp parse.cpp)
