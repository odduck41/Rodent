cmake_minimum_required(VERSION 3.28.1)
project(Rodent)

set(CMAKE_CXX_STANDARD 20)

add_executable(Rodent
        main.cpp
        lexer.hpp
        lexer.cpp
        tests.h
)
