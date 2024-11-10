#pragma once

#include "lexer.hpp"
#include <iostream>
#include <cassert>

inline void Trie() {
    lexer::Trie a("../assets/reserved.txt");
    lexer::Trie b("../assets/types.txt");

    assert(a.check(L"import") == true);

    assert(a.check(L"func") == true);
    assert(a.check(L"return") == true);

    assert(a.check(L"do") == true);
    assert(a.check(L"while") == true);
    assert(a.check(L"for") == true);


    assert(a.check(L"continue") == true);
    assert(a.check(L"break") == true);


    assert(a.check(L"if") == true);
    assert(a.check(L"else") == true);
    assert(a.check(L"elif") == true);


    assert(a.check(L"switch") == true);
    assert(a.check(L"case") == true);



    assert(a.check(L"impo") == false);

    assert(a.check(L"fun678c") == false);
    assert(a.check(L"retur8n") == false);

    assert(a.check(L"dofj") == false);
    assert(a.check(L"whifje") == false);
    assert(a.check(L"fofjr") == false);


    assert(a.check(L"contin") == false);
    assert(a.check(L"bre") == false);


    assert(a.check(L"iffgj") == false);
    assert(a.check(L"elsejg") == false);
    assert(a.check(L"elifghj") == false);


    assert(a.check(L"switchfgj") == false);
    assert(a.check(L"ca") == false);

    assert(b.check(L"int") == true);
    assert(b.check(L"double") == true);
    // assert(b.check(L"wchar_t") == true);
    /// Блять Артём Ну ты и долбоёб конечно, сука, алё, какой wchar_t, в самом языке его не будет
    /// Оно есть только у нас, в языке просто str
    assert(b.check(L"bool") == true);
    assert(b.check(L"str")== true);
    assert(b.check(L"array") == true);


    assert(b.check(L"i") == false);
    assert(b.check(L"doubl") == false);
    assert(b.check(L"cha") == false);
    assert(b.check(L"boo") == false);
    assert(b.check(L"std") == false);
    assert(b.check(L"arra") == false);
}

inline void operations() {
    std::wstring a;
    // >>=&&|=||=-=**=*->
    std::getline(std::wcin, a);
    auto ans = lexer::parseOperations(a, 1);
    for (auto& i : ans) {
        std::wcout << i.content << std::endl;
    }
}