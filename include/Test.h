#pragma once

#include "lexer.hpp"
#include <cassert>

inline void test() {
    assert(inTrie("import", reserved) == true);

    assert(inTrie("func", reserved) == true);
    assert(inTrie("return", reserved) == true);

    assert(inTrie("do", reserved) == true);
    assert(inTrie("while", reserved) == true);
    assert(inTrie("for", reserved) == true);


    assert(inTrie("continue", reserved) == true);
    assert(inTrie("break", reserved) == true);


    assert(inTrie("if", reserved) == true);
    assert(inTrie("else", reserved) == true);
    assert(inTrie("elif", reserved) == true);


    assert(inTrie("switch", reserved) == true);
    assert(inTrie("case", reserved) == true);



    assert(inTrie("impo", reserved) == false);

    assert(inTrie("fun678c", reserved) == false);
    assert(inTrie("retur8n", reserved) == false);

    assert(inTrie("dofj", reserved) == false);
    assert(inTrie("whifje", reserved) == false);
    assert(inTrie("fofjr", reserved) == false);


    assert(inTrie("contin", reserved) == false);
    assert(inTrie("bre", reserved) == false);


    assert(inTrie("iffgj", reserved) == false);
    assert(inTrie("elsejg", reserved) == false);
    assert(inTrie("elifghj", reserved) == false);


    assert(inTrie("switchfgj", reserved) == false);
    assert(inTrie("ca", reserved) == false);

    assert(inTrie("int", types) == true);
    assert(inTrie("double", types) == true);
    assert(inTrie("char", types) == true);
    assert(inTrie("bool", types) == true);
    assert(inTrie("str", types) == true);
    assert(inTrie("array", types) == true);


    assert(inTrie("i", types) == false);
    assert(inTrie("doubl", types) == false);
    assert(inTrie("cha", types) == false);
    assert(inTrie("boo", types) == false);
    assert(inTrie("std", types) == false);
    assert(inTrie("arra", types) == false);
}