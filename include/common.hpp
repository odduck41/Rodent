#pragma once

#include <map>
#include <iostream>

#include "exceptions.hpp"
#include "files.hpp"
#include "lexer.hpp"

template<class ...T>
std::map<char, void(*)(T ...)> function;

std::string filename;
std::string out;


void lex(const char* output = "a.dev") {
    out = output;
}

static inline void defineFlags() {
    function<const char*>['l'] = &lex;
}

static inline void doAll() {

    freopen("hello.txt", "w", stdout);

    RFile file(filename.c_str());
    const long long size_ = file.size();

    char* program = new char[size_ + 1];

    file.read(program, size_);

    program[size_] = '\0';

    removeComments(size_ + 1, program);



    delete[] program;
}

inline void doFlags(int argc, const char** argv) {
    defineFlags();
    for (size_t i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            if (function<>.contains(argv[i][1]) || function<const char*>.contains(argv[i][1])) {
                if (i + 1 != argc && argv[i + 1][0] != '-') {
                    function<const char*>[argv[i][1]](argv[i + 1]);
                } else {
                    function<>[argv[i][1]]();
                }

                goto next;
            }
            throw BadFlag(argv[i]);
        } else {
             filename = argv[i];
        }

        next:;
    }

    doAll();
}

