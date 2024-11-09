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
