#pragma once

#include "lexer.hpp"
#include <iostream>
#include <cassert>

inline void Trie() {
    const lexer::Trie reserved("../assets/reserved.txt");
    const lexer::Trie types("../assets/types.txt");

    assert(reserved.check(L"import") == true);

    assert(reserved.check(L"func") == true);
    assert(reserved.check(L"return") == true);

    assert(reserved.check(L"do") == true);
    assert(reserved.check(L"while") == true);
    assert(reserved.check(L"for") == true);


    assert(reserved.check(L"continue") == true);
    assert(reserved.check(L"break") == true);


    assert(reserved.check(L"if") == true);
    assert(reserved.check(L"else") == true);
    assert(reserved.check(L"elif") == true);


    assert(reserved.check(L"switch") == true);
    assert(reserved.check(L"case") == true);



    assert(reserved.check(L"impo") == false);

    assert(reserved.check(L"fun678c") == false);
    assert(reserved.check(L"retur8n") == false);

    assert(reserved.check(L"dofj") == false);
    assert(reserved.check(L"whifje") == false);
    assert(reserved.check(L"fofjr") == false);


    assert(reserved.check(L"contin") == false);
    assert(reserved.check(L"bre") == false);


    assert(reserved.check(L"iffgj") == false);
    assert(reserved.check(L"elsejg") == false);
    assert(reserved.check(L"elifghj") == false);


    assert(reserved.check(L"switchfgj") == false);
    assert(reserved.check(L"ca") == false);

    assert(types.check(L"int") == true);
    assert(types.check(L"double") == true);
    // assert(b.check(L"wchar_t") == true);
    /// Блять Артём Ну ты и долбоёб конечно, сука, алё, какой wchar_t, в самом языке его не будет
    /// Оно есть только у нас, в языке просто str
    assert(types.check(L"bool") == true);
    assert(types.check(L"str")== true);
    assert(types.check(L"array") == true);


    assert(types.check(L"i") == false);
    assert(types.check(L"doubl") == false);
    assert(types.check(L"cha") == false);
    assert(types.check(L"boo") == false);
    assert(types.check(L"std") == false);
    assert(types.check(L"arra") == false);
}

inline void operations() {
    std::wstring a;
    // >>=&&|=||=-=**=*->
    std::getline(std::wcin, a);
    for (auto ans = lexer::parseOperations(a, 1); auto&[type, content, line] : ans) {
        std::wcout << content << std::endl;
    }
}
