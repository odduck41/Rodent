#include "lexer.hpp"
#include "include/tests.h"



int main(int argc, char** argv) {
    load_reserved();
    load_basic_types();
    test();
    return 0;
}
