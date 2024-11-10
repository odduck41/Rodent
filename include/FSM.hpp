#pragma once

#include "lexer.hpp"

#include <iostream>
#include <string>
#include <vector>
#include "basic.hpp"

namespace lexer {

namespace states {
struct State {
  std::wstring curr_str;
  virtual bool is_begin() = 0;
  State() = default;
  State(const std::wstring& str) : curr_str(str) {}
};
struct Begin final : virtual State {
  Begin() = default;
  bool is_begin() {return true;};
  Begin(const std::wstring& str) : State(str) {}
};
struct RTI final : virtual State {
  RTI() = default;
  bool is_begin() {return true;};
  RTI(const std::wstring& str) : State(str) {}
};
struct Literal final : virtual State {
  Literal() = default;
  bool is_begin() {return true;};
  Literal(const std::wstring& str) : State(str) {}
};
struct StringLiteral final : virtual State {
  StringLiteral() = default;
  bool is_begin() {return true;};
  StringLiteral(const std::wstring& str) : State(str) {}
};
struct SpecialSymbols final : virtual State {
  SpecialSymbols() = default;
  bool is_begin() {return true;};
  SpecialSymbols(const std::wstring& str) : State(str) {}
};
struct Other final : virtual State {
  Other() = default;
  bool is_begin() {return true;};
  Other(const std::wstring& str) : State(str) {}
};
}  // namespace states

namespace events {
struct Event {
  wchar_t symbol;
  virtual void not_used() = 0;
};
struct Letter final : virtual Event {
  virtual void not_used(){}
};
struct DecimalDigit final : virtual Event {
  virtual void not_used(){}
};
struct AllBasesDigit final : virtual Event {
  virtual void not_used(){}
};
struct BaseSeparator final : virtual Event {
  virtual void not_used(){}
};
struct Dot final : virtual Event {
  virtual void not_used(){}
};
struct Space final : virtual Event {
  virtual void not_used(){}
};
struct Curly final : virtual Event {
  virtual void not_used(){}
};
struct Parentheses final : virtual Event {
  virtual void not_used(){}
};
struct Quotes final : virtual Event {
  virtual void not_used(){}
};
struct Underline final : virtual Event {
  virtual void not_used(){}
};
struct Operation final : virtual Event {
  virtual void not_used(){}
};
struct Semicolor final : virtual Event {
  virtual void not_used(){}
};
}  // namespace events

class FiniteStateMachine {
 public:
  FiniteStateMachine(const wchar_t* programm_text, size_t programm_lenght);

  std::vector<Token> getTokens();

  ~FiniteStateMachine() = default;

 private:
  const wchar_t* program_text_;
  size_t program_lenght_;
  std::vector<Token> tokens_;
  size_t curr_line_ = 1;
  Trie reserved_words_;
  Trie type_words_;

  void startProcessText();

  events::Event* getEvent(const wchar_t symbol) const;

  void operations(const std::wstring& str);

  void applyState(states::Other* state);
  void applyState(states::SpecialSymbols* state);
  void applyState(states::RTI* state);
  void applyState(states::Literal* state);
  void applyState(states::StringLiteral* state);
  // Блять, Артём, я конечно всё понимаю, но пиши, пожалуйста свои комментарии
  // так, что они не кидали Typo

  // Ааахxахахa, ещe чегo мнe делaть
  void applyState(states::State* state);  // Кидвет искоючение

  // Извините конечно, но этот пиздец надо было сделать
  // Begin
  states::State* onEvent(states::Begin* state, events::Space* event);
  states::State* onEvent(states::Begin* state, events::Curly* event);
  states::State* onEvent(states::Begin* state, events::Parentheses* event);
  states::State* onEvent(states::Begin* state, events::Quotes* event);
  states::State* onEvent(states::Begin* state, events::DecimalDigit* event);
  states::State* onEvent(states::Begin* state, events::Letter* event);
  states::State* onEvent(states::Begin* state, events::Operation* event);
  states::State* onEvent(states::Begin* state, events::Event* event);

  // StringLiteral
  states::State* onEvent(states::StringLiteral* state, events::Quotes* event);
  states::State* onEvent(states::StringLiteral* state, events::Event* event);

  // Literal
  states::State* onEvent(states::Literal* state, events::Dot* event);
  states::State* onEvent(states::Literal* state, events::BaseSeparator* event);
  states::State* onEvent(states::Literal* state, events::AllBasesDigit* event);
  states::State* onEvent(states::Literal* state, events::Event* event);

  // RTI
  states::State* onEvent(states::RTI* state, events::Dot* event);
  states::State* onEvent(states::RTI* state, events::Letter* event);
  states::State* onEvent(states::RTI* state, events::DecimalDigit* event);
  states::State* onEvent(states::RTI* state, events::Underline* event);
  states::State* onEvent(states::RTI* state, events::Event* event);

  // Operation
  states::State* onEvent(states::SpecialSymbols* state, events::Dot* event);
  states::State* onEvent(states::SpecialSymbols* state, events::Operation* event);
  states::State* onEvent(states::SpecialSymbols* state, events::Event* event);

  // Other
  states::State* onEvent(states::Other* state, events::Event* event);

  // Default
  // states::State onEvent(states::State* state, events::Event* event);
};
}  // namespace lexer
