#pragma once

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

struct SemUnit {
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
