#pragma once

#include <string>
#include <vector>


enum class State {
  S,
  RTI, // Reserved, Type, Identification
  Literal,
  Dot,
  SLiteral,
  EoSLiteral,
  Other
};

enum class Lexeme {
  Reserved,
  Identifier,
  Literal,
  Type,
  SLiteral,
  Operation,
  Punctuation,
  Dot,
  Comma,
  OParentheses,
  CParentheses,
  OCurly,
  CCurly,
  OSquare,
  CSquare,
  Other
};

struct Token {
  Lexeme type{Lexeme::Other};
  std::wstring content{};
  size_t line{};
};

extern std::vector<Token> tokens;

void stateMachine(const wchar_t*w);
