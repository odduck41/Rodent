#pragma once
#include <lexer.hpp>

#include <iostream>
void test() {
    std::cout << inTrie("return") << std::endl;
    std::cout << inTrie("func") << std::endl;

    std::cout << inTrie("while") << std::endl;
    std::cout << inTrie("for") << std::endl;
    std::cout << inTrie("do") << std::endl;
    std::cout << inTrie("continue") << std::endl;
    std::cout << inTrie("break") << std::endl;


    std::cout << inTrie("if") << std::endl;
    std::cout << inTrie("else") << std::endl;
    std::cout << inTrie("elif") << std::endl;

    std::cout << inTrie("enum") << std::endl;
    std::cout << inTrie("class") << std::endl;
    std::cout << inTrie("union") << std::endl;
    std::cout << inTrie("private") << std::endl;
    std::cout << inTrie("public") << std::endl;

    std::cout << inTrie("import") << std::endl;


    std::cout << inTrie("iemporet") << std::endl;
    std::cout << inTrie("importe") << std::endl;
    std::cout << inTrie("ava") << std::endl;
}