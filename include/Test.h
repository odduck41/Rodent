#pragma once

#include "lexer.hpp"
#include <cassert>

inline void test() {
    assert(inTrie(L"import", reserved) == true);

    assert(inTrie(L"func", reserved) == true);
    assert(inTrie(L"return", reserved) == true);

    assert(inTrie(L"do", reserved) == true);
    assert(inTrie(L"while", reserved) == true);
    assert(inTrie(L"for", reserved) == true);


    assert(inTrie(L"continue", reserved) == true);
    assert(inTrie(L"break", reserved) == true);


    assert(inTrie(L"if", reserved) == true);
    assert(inTrie(L"else", reserved) == true);
    assert(inTrie(L"elif", reserved) == true);


    assert(inTrie(L"switch", reserved) == true);
    assert(inTrie(L"case", reserved) == true);



    assert(inTrie(L"impo", reserved) == false);

    assert(inTrie(L"fun678c", reserved) == false);
    assert(inTrie(L"retur8n", reserved) == false);

    assert(inTrie(L"dofj", reserved) == false);
    assert(inTrie(L"whifje", reserved) == false);
    assert(inTrie(L"fofjr", reserved) == false);


    assert(inTrie(L"contin", reserved) == false);
    assert(inTrie(L"bre", reserved) == false);


    assert(inTrie(L"iffgj", reserved) == false);
    assert(inTrie(L"elsejg", reserved) == false);
    assert(inTrie(L"elifghj", reserved) == false);


    assert(inTrie(L"switchfgj", reserved) == false);
    assert(inTrie(L"ca", reserved) == false);

    assert(inTrie(L"int", types) == true);
    assert(inTrie(L"double", types) == true);
    assert(inTrie(L"wchar_t", types) == true);
    assert(inTrie(L"bool", types) == true);
    assert(inTrie(L"str", types) == true);
    assert(inTrie(L"array", types) == true);


    assert(inTrie(L"i", types) == false);
    assert(inTrie(L"doubl", types) == false);
    assert(inTrie(L"cha", types) == false);
    assert(inTrie(L"boo", types) == false);
    assert(inTrie(L"std", types) == false);
    assert(inTrie(L"arra", types) == false);
}