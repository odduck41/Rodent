cmake_minimum_required(VERSION 3.28.1)
project(Rodent C)

set(CMAKE_C_STANDARD 11)

add_executable(Rodent
        main.c
        lexer.h
)
