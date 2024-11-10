#include "common.hpp"
#include "parser.hpp"
#include "Test.h"

// Лол, если basic.hpp - первый include, то оно нахуй падает, хотя везде есть #pragma once...
#include "basic.hpp"
// #include "FSM.hpp"
// #include <exceptions.hpp>

int main(int argc, const char** argv) {
  doFlags(argc, argv);
  // Trie();
  // operations();

  doAll();
  // doFlags(argc, argv);
  // Trie();
  // operations();
  std::vector a = {
    Token{Lexeme::Reserved, L"import", 1},
    Token{Lexeme::StringLiteral, L"abc.rod", 1},

    Token{Lexeme::Reserved, L"func", 2},
    Token{Lexeme::Identifier, L"gcd", 2},
    Token{Lexeme::OpenParentheses, L"(", 2},
    Token{Lexeme::Type, L"int", 2},
    Token{Lexeme::Identifier, L"a", 2},
    Token{Lexeme::Punctuation, L",", 2},
    Token{Lexeme::Type, L"int", 2},
    Token{Lexeme::Identifier, L"b", 2},
    Token{Lexeme::CloseParentheses, L")", 2},
    Token{Lexeme::Operation, L"->", 2},
    Token{Lexeme::Type, L"int", 2},
    Token{Lexeme::OpenCurly, L"{", 2},

    Token{Lexeme::Reserved, L"while", 3},
    Token{Lexeme::OpenParentheses, L"(", 3},
    Token{Lexeme::Identifier, L"a", 3},
    Token{Lexeme::Operation, L"&&", 3},
    Token{Lexeme::Identifier, L"b", 3},
    Token{Lexeme::CloseParentheses, L")", 3},
    Token{Lexeme::OpenCurly, L"{", 3},

    Token{Lexeme::Reserved, L"do", 4},
    Token{Lexeme::OpenCurly, L"{", 4},
    Token{Lexeme::CloseCurly, L"}", 4},
    Token{Lexeme::Reserved, L"while", 4},
    Token{Lexeme::OpenParentheses, L"(", 4},
    Token{Lexeme::Literal, L"0", 4},
    Token{Lexeme::CloseParentheses, L")", 4},
    Token{Lexeme::Semicolon, L";", 4},

    Token{Lexeme::Reserved, L"if", 5},
    Token{Lexeme::OpenParentheses, L"(", 5},
    Token{Lexeme::Identifier, L"a", 5},
    Token{Lexeme::Operation, L">", 5},
    Token{Lexeme::Identifier, L"b", 5},
    Token{Lexeme::CloseParentheses, L")", 5},
    Token{Lexeme::OpenCurly, L"{", 5},

    Token{Lexeme::Identifier, L"a", 6},
    Token{Lexeme::Operation, L"&=", 6},
    Token{Lexeme::Literal, L"4", 6},

    Token{Lexeme::Semicolon, L";", 6},

    Token{Lexeme::Identifier, L"a", 7},
    Token{Lexeme::Operation, L"%", 7},
    Token{Lexeme::Identifier, L"b", 7},

    Token{Lexeme::Semicolon, L";", 7},

    Token{Lexeme::CloseCurly, L"}", 8},

    Token{Lexeme::Reserved, L"else", 8},
    Token{Lexeme::OpenCurly, L"{", 8},
    Token{Lexeme::Identifier, L"b", 9},
    Token{Lexeme::Operation, L"=", 9},
    Token{Lexeme::Identifier, L"a", 9},

    Token{Lexeme::Semicolon, L";", 9},

    Token{Lexeme::Identifier, L"c", 10},
    Token{Lexeme::Operation, L"==", 10},
    Token{Lexeme::Identifier, L"a", 10},

    Token{Lexeme::Semicolon, L";", 10},
    Token{Lexeme::CloseCurly, L"}", 11},

    Token{Lexeme::CloseCurly, L"}", 11},

    Token{Lexeme::Reserved, L"return", 12},
    Token{Lexeme::Literal, L"34", 12},

    Token{Lexeme::Semicolon, L";", 12},
    Token{Lexeme::CloseCurly, L"}", 13},

    Token{Lexeme::Reserved, L"func", 13},
    Token{Lexeme::Identifier, L"main", 13},
    Token{Lexeme::OpenParentheses, L"(", 13},
    Token{Lexeme::CloseParentheses, L")", 13},
    Token{Lexeme::Operation, L"->", 13},
    Token{Lexeme::Type, L"int", 13},
    Token{Lexeme::OpenCurly, L"{", 13},

    Token{Lexeme::Identifier, L"gcd", 14},
    Token{Lexeme::OpenParentheses, L"(", 14},
    Token{Lexeme::Identifier, L"a", 14},
    Token{Lexeme::Punctuation, L",", 14},
    Token{Lexeme::Identifier, L"b", 14},
    Token{Lexeme::CloseParentheses, L")", 14},

    Token{Lexeme::Semicolon, L";", 14},

    Token{Lexeme::Identifier, L"gcd2", 15},
    Token{Lexeme::OpenParentheses, L"(", 15},
    Token{Lexeme::Literal, L"2", 15},
    Token{Lexeme::Punctuation, L",", 15},
    Token{Lexeme::Identifier, L"b", 15},
    Token{Lexeme::CloseParentheses, L")", 15},
    Token{Lexeme::Semicolon, L";", 15},

    Token{Lexeme::Reserved, L"do", 16},
    Token{Lexeme::OpenCurly, L"{", 16},

    Token{Lexeme::Identifier, L"a", 17},
    Token{Lexeme::Operation, L"+=", 17},
    Token{Lexeme::Literal, L"1", 17},
    Token{Lexeme::Semicolon, L";", 17},
    Token{Lexeme::CloseCurly, L"}", 18},

    Token{Lexeme::Reserved, L"while", 18},
    Token{Lexeme::OpenParentheses, L"(", 18},
    Token{Lexeme::Identifier, L"a", 18},
    Token{Lexeme::Operation, L"==", 18},
    Token{Lexeme::Literal, L"1", 18},
    Token{Lexeme::CloseParentheses, L")", 18},
    Token{Lexeme::Semicolon, L";", 18},

    Token{Lexeme::CloseCurly, L"}", 20},

    Token{Lexeme::Reserved, L"func", 21},
    Token{Lexeme::Identifier, L"gcd2", 21},
    Token{Lexeme::OpenParentheses, L"(", 21},
    Token{Lexeme::Type, L"int", 21},
    Token{Lexeme::Identifier, L"a", 21},
    Token{Lexeme::Punctuation, L",", 21},
    Token{Lexeme::Type, L"int", 21},
    Token{Lexeme::Identifier, L"b", 21},
    Token{Lexeme::CloseParentheses, L")", 21},
    Token{Lexeme::Operation, L"->", 21},
    Token{Lexeme::Type, L"int", 21},

    Token{Lexeme::OpenCurly, L"{", 21},

    Token{Lexeme::Reserved, L"return", 22},
    Token{Lexeme::Identifier, L"a", 22},
    Token{Lexeme::Operation, L"+", 22},
    Token{Lexeme::Identifier, L"b", 22},

    Token{Lexeme::Semicolon, L";", 22},
    Token{Lexeme::CloseCurly, L"}", 23}
  };

  try {
    Parser e(a);
  } catch (bad_lexeme& err) {
    std::wcerr << err.what(0);
    return -1;
  }
  // HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  // SetConsoleTextAttribute(hConsole, 10);
  std::cout << "OK";
  return 0;
}