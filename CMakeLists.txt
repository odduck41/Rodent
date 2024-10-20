cmake_minimum_required(VERSION 3.29)
project(Rodent C)

set(CMAKE_C_STANDARD 11)

add_executable(Rodent
        main.c
        lexer.h)
