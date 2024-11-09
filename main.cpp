#include "common.hpp"
#include "parser.hpp"
#include "Test.h"
// #include "FSM.hpp"
// #include <exceptions.hpp>

int main(int argc, const char** argv) {

  // doFlags(argc, argv);
  // Trie();
  // operations();
  std::vector a = {
    Token{Lexeme::Reserved, L"func", 1},
    Token{Lexeme::Identifier, L"gcd", 1},
    Token{Lexeme::OpenParentheses, L"(", 1},
    Token{Lexeme::Type, L"int", 1},
    Token{Lexeme::Identifier, L"a", 1},
    Token{Lexeme::Punctuation, L",", 1},
    Token{Lexeme::Type, L"int", 1},
    Token{Lexeme::Identifier, L"b", 1},
    Token{Lexeme::CloseParentheses, L")", 1},
    Token{Lexeme::Operation, L"->", 1},
    Token{Lexeme::Type, L"int", 1},
    Token{Lexeme::OpenCurly, L"{", 1},

    Token{Lexeme::Reserved, L"while", 2},
    Token{Lexeme::OpenParentheses, L"(", 2},
    Token{Lexeme::Identifier, L"a", 2},
    Token{Lexeme::Operation, L"&&", 2},
    Token{Lexeme::Identifier, L"b", 2},
    Token{Lexeme::CloseParentheses, L")", 2},

    Token{Lexeme::Reserved, L"if", 3},
    Token{Lexeme::OpenParentheses, L"(", 3},
    Token{Lexeme::Identifier, L"a", 3},
    Token{Lexeme::Operation, L">", 3},
    Token{Lexeme::Identifier, L"b", 3},
    Token{Lexeme::CloseParentheses, L")", 3},

    Token{Lexeme::Identifier, L"a", 4},
    Token{Lexeme::Operation, L"%=", 4},
    Token{Lexeme::Identifier, L"b", 4},
    Token{Lexeme::Semicolon, L";", 4},

    Token{Lexeme::Reserved, L"else", 5},
    Token{Lexeme::OpenParentheses, L"(", 5},
    Token{Lexeme::CloseParentheses, L")", 5},

    Token{Lexeme::Identifier, L"b", 6},
    Token{Lexeme::Operation, L"%=", 6},
    Token{Lexeme::Identifier, L"a", 6},
    Token{Lexeme::Semicolon, L";", 6},

    Token{Lexeme::CloseCurly, L"}", 7},

    Token{Lexeme::Reserved, L"return", 8},
    Token{Lexeme::Identifier, L"a", 8},
    Token{Lexeme::Operation, L"+", 8},
    Token{Lexeme::Identifier, L"b", 8},
    Token{Lexeme::Semicolon, L";", 8},

    Token{Lexeme::CloseCurly, L"}", 9},

    Token{Lexeme::Reserved, L"func", 10},
    Token{Lexeme::Identifier, L"main", 10},
    Token{Lexeme::OpenParentheses, L"(", 10},
    Token{Lexeme::CloseParentheses, L")", 10},
    Token{Lexeme::Reserved, L"->", 10},
    Token{Lexeme::Type, L"int", 10},
    Token{Lexeme::OpenCurly, L"{", 10},

    Token{Lexeme::Reserved, L"return", 11},
    Token{Lexeme::Identifier, L"gcd", 11},
    Token{Lexeme::OpenParentheses, L"(", 11},
    Token{Lexeme::Literal, L"543", 11},
    Token{Lexeme::Punctuation, L",", 11},
    Token{Lexeme::Literal, L"4532", 11},
    Token{Lexeme::CloseParentheses, L")", 11},
    Token{Lexeme::Semicolon, L";", 11},

    Token{Lexeme::CloseCurly, L"}", 12},
  };

  Parser e(a);
  return 0;
}