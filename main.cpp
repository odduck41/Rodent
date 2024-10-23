#include "lexer.hpp"

int main(int argc, char** argv) {
    char** val;
    asm("mov %0, %1" : "=r" (val) : "r" (argv));

    return 0;
}
