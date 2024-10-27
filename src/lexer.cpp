#include <cstddef>
#include "lexer.hpp"
#include <string>

void removeComments(size_t sz, char* program) {
    std::string result;
    result.reserve(sz + 1);

    bool single{}, multiline{};
    for (size_t i = 0; program[i] != '\0'; ++i) {
        if (program[i] == '\r') {
            continue;
        }

        if (single) {
            if (program[i] == '\n') {
                single = false;
            } else {
                continue;
            }
        }

        if (multiline) {
            if (program[i] == '\n') {
                result += program[i];
                continue;
            }

            if (program[i] == '*' && program[i + 1] == '/') {
                multiline = false;
                ++i;
            }

            continue;
        }

        if (program[i] == '/' && program[i + 1] == '/') {
            single = true;
            continue;
        }

        if (program[i] == '/' && program[i + 1] == '*') {
            multiline = true;
            continue;
        }

        result += program[i];
    }


    result.copy(program, result.size());
    program[result.size()] = '\0';
}