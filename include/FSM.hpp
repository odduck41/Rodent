#pragma once

#include "lexer.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace lexer {
// std::vector<Token> operations(const std::string&, size_t);

namespace states {
struct State {
  std::wstring curr_str;
};
struct Begin : virtual State {};
struct RTI : virtual State {};
struct Literal : virtual State {};
struct StringLiteral : virtual State {};
struct SpecialSymbols : virtual State {};
struct Other : virtual State {};
}  // namespace states

namespace events {
struct Event {
  char symbol;
};
struct Letter : virtual Event {};
struct DecimalDigit : virtual Event {};
struct AllBasesDigit : virtual Event {};
struct BaseSeparator : virtual Event {};
struct Dot : virtual Event {};
struct Space : virtual Event {};
struct Curly : virtual Event {};
struct Parentheses : virtual Event {};
struct Quotes : virtual Event {};
struct Underline : virtual Event {};
struct Operation : virtual Event {};
struct Semicolor : virtual Event {};
}  // namespace events

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

class FiniteStateMachine {
 public:
  FiniteStateMachine(const char* programm_text, size_t programm_lenght);

  std::vector<Token> getTokens();

  ~FiniteStateMachine() = default;

 private:
  const char* programm_text_;
  size_t programm_lenght_;
  std::vector<Token> tokens_;
  size_t curr_line_ = 1;
  Trie reserwed_words_;
  Trie type_words_;

  void startProcessText();

  void operations(const std::wstring& str);

  void applyState(states::Other state);
  void applyState(states::SpecialSymbols state);
  void applyState(states::RTI state);
  void applyState(states::Literal state);
  void applyState(states::StringLiteral state);
  void applyState(states::State state);  // Кидвет искоючение

  // Извините конечно, но этот пиздец надо было сделать
  // Begin
  states::State onEvent(states::Begin state, events::Space event) {}
  states::State onEvent(states::Begin state, events::Curly event) {}
  states::State onEvent(states::Begin state, events::Parentheses event) {}
  states::State onEvent(states::Begin state, events::Quotes event) {}
  states::State onEvent(states::Begin state, events::DecimalDigit event) {}
  states::State onEvent(states::Begin state, events::Letter event) {}
  states::State onEvent(states::Begin state, events::Operation event) {}
  states::State onEvent(states::Begin state, events::Event event) {}

  // StringLiteral
  states::State onEvent(states::StringLiteral state, events::Quotes event) {}
  states::State onEvent(states::StringLiteral state, events::Event event) {}

  // Literal
  states::State onEvent(states::Literal state, events::Dot event) {}
  states::State onEvent(states::Literal state, events::BaseSeparator event) {}
  states::State onEvent(states::Literal state, events::AllBasesDigit event) {}
  states::State onEvent(states::Literal state, events::Event event) {}

  // RTI
  states::State onEvent(states::RTI state, events::Dot event) {}
  states::State onEvent(states::RTI state, events::Letter event) {}
  states::State onEvent(states::RTI state, events::DecimalDigit event) {}
  states::State onEvent(states::RTI state, events::Underline event) {}
  states::State onEvent(states::RTI state, events::Event event) {}

  // Operation
  states::State onEvent(states::SpecialSymbols state, events::Dot event) {}
  states::State onEvent(states::SpecialSymbols state, events::Operation event) {
  }
  states::State onEvent(states::SpecialSymbols state, events::Event event) {}

  // Other
  states::State onEvent(states::Other state, events::Event event) {}

  // Default
  states::State onEvent(states::State state, events::Event event) {}
};
}  // namespace lexer
