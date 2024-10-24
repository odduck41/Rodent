#pragma once
#include <lexer.hpp>

#include <iostream>
void test() {
    std::cout << inTrie("return", reserved_) << std::endl;
    std::cout << inTrie("func", reserved_) << std::endl;

    std::cout << inTrie("while", reserved_) << std::endl;
    std::cout << inTrie("for", reserved_) << std::endl;
    std::cout << inTrie("do", reserved_) << std::endl;
    std::cout << inTrie("continue", reserved_) << std::endl;
    std::cout << inTrie("break", reserved_) << std::endl;


    std::cout << inTrie("if", reserved_) << std::endl;
    std::cout << inTrie("else", reserved_) << std::endl;
    std::cout << inTrie("elif", reserved_) << std::endl;

    std::cout << inTrie("enum", reserved_) << std::endl;
    std::cout << inTrie("class", reserved_) << std::endl;
    std::cout << inTrie("union", reserved_) << std::endl;
    std::cout << inTrie("private", reserved_) << std::endl;
    std::cout << inTrie("public", reserved_) << std::endl;

    std::cout << inTrie("import", reserved_) << std::endl;


    std::cout << inTrie("iemporet", reserved_) << std::endl;
    std::cout << inTrie("importe", reserved_) << std::endl;
    std::cout << inTrie("ava", reserved_) << std::endl;

    std::cout << inTrie("int", types_) << std::endl;
    std::cout << inTrie("str", types_) << std::endl;
    std::cout << inTrie("char", types_) << std::endl;
    std::cout << inTrie("bool", types_) << std::endl;
    std::cout << inTrie("array", types_) << std::endl;
    std::cout << inTrie("double", types_) << std::endl;
}