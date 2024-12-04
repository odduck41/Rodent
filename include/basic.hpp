#pragma once
#include <map>
#include <string>


using Type = std::wstring;
using Name = std::wstring;
using Variable = std::pair<Name, Type>;


enum class Lexeme {
    Reserved,
    Identifier,
    Type,
    Literal,
    StringLiteral,
    Operation,
    Punctuation,
    Semicolon,
    OpenParentheses,
    CloseParentheses,
    OpenCurly,
    CloseCurly,
    Other
};

struct Token {
    Lexeme type{Lexeme::Other};
    std::wstring content{};
    size_t line{};
};

// Больше не нужон
// inline std::wstring& operator+= (std::wstring& s, const Lexeme& l) {
//     if (l == Lexeme::Reserved) s += L"Reserved";
//     else if (l == Lexeme::Identifier) s += L"Identifier";
//     else if (l == Lexeme::Type) s += L"Type";
//     else if (l == Lexeme::Literal) s += L"Literal";
//     else if (l == Lexeme::StringLiteral) s += L"StringLiteral";
//     else if (l == Lexeme::Operation) s += L"Operation";
//     else if (l == Lexeme::Punctuation) s += L"Punctuation";
//     else if (l == Lexeme::Semicolon) s += L"Semicolon";
//     else if (l == Lexeme::OpenParentheses) s += L"Open Parentheses";
//     else if (l == Lexeme::CloseParentheses) s += L"Close Parentheses";
//     else if (l == Lexeme::OpenCurly) s += L"Open Curly";
//     else if (l == Lexeme::CloseCurly) s += L"Close Curly";
//     else s += L"Other";
//
//     return s;
// }

inline std::wstring asWstring(const Lexeme& l) {
    std::wstring s;
    if (l == Lexeme::Reserved) s = L"Reserved";
    else if (l == Lexeme::Identifier) s = L"Identifier";
    else if (l == Lexeme::Type) s = L"Type";
    else if (l == Lexeme::Literal) s = L"Literal";
    else if (l == Lexeme::StringLiteral) s = L"StringLiteral";
    else if (l == Lexeme::Operation) s = L"Operation";
    else if (l == Lexeme::Punctuation) s = L"Punctuation";
    else if (l == Lexeme::Semicolon) s = L"Semicolon";
    else if (l == Lexeme::OpenParentheses) s = L"OpenParentheses";
    else if (l == Lexeme::CloseParentheses) s = L"CloseParentheses";
    else if (l == Lexeme::OpenCurly) s = L"OpenCurly";
    else if (l == Lexeme::CloseCurly) s = L"CloseCurly";
    else s = L"Other";

    return s;
}

// Я скопировал это из ветки Semantic, потому что я ебал это писать
// Но map по-моему лучше чем бесконечность if
inline std::map<Type, std::map<Type, Type>> transformations = {
    {
        L"int",
        {
                {L"char", L"int"},
                {L"double", L"double"},
                {L"bool", L"int"},
                {L"int", L"int"}
        }
    },

    {
        L"char",
        {
                {L"int", L"int"},
                {L"double", L"double"},
                {L"bool", L"char"},
                {L"char", L"str"},
                // {L"str", L"str"}
            }
    },

    {
        L"double",
        {
                {L"int", L"double"},
                {L"double", L"double"},
                {L"bool", L"double"},
                {L"char", L"double"}
        }
    },

    {
        L"bool",
        {
                {L"int", L"int"},
                {L"double", L"double"},
                {L"bool", L"bool"},
                {L"char", L"char"},
            }
    },

    {L"str",
        {
        {L"str", L"str"},
        {L"char", L"str"}
      }
    }
};


inline bool isComingDown(Type a, Type b) { // a -> b
    if (b.back() == '&' && a.back() != '&') return false;
    if (a.back() == '&') {
        a = a.substr(0, a.size() - 1);
    }
    if (b.back() == '&') {
        b = b.substr(0, b.size() - 1);
    }
    return transformations[a].contains(b);
}