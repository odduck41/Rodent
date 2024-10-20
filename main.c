#include <stdio.h>
#include "lexer.h"

int main(int argc, char** argv) {
    char** val;
    asm("mov %0, %1" : "=r" (val) : "r" (argv));

    printf("%s", val[argc - 1]);
    return 0;
}
