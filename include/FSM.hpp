#pragma once

#include "lexer.hpp"
#include "basic.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <variant>

namespace lexer {

namespace states {
struct Begin {
  std::wstring curr_str;
  Begin() = default;
  explicit Begin(const std::wstring& str) : curr_str(str) {}
  void operator()() const {};
};
struct RTI {
  std::wstring curr_str;
  RTI() = default;
  explicit RTI(const std::wstring& str) : curr_str(str) {}
  void operator()() const {};
};
struct Literal {
  std::wstring curr_str;
  Literal() = default;
  explicit Literal(const std::wstring& str) : curr_str(str) {}
  void operator()() const {};
};
struct StringLiteral {
  std::wstring curr_str;
  StringLiteral() = default;
  explicit StringLiteral(const std::wstring& str) : curr_str(str) {}
  void operator()() const {};
};
struct SpecialSymbols {
  std::wstring curr_str;
  SpecialSymbols() = default;
  explicit SpecialSymbols(const std::wstring& str) : curr_str(str) {}
  void operator()() const {};
};
struct Other {
  std::wstring curr_str;
  Other() = default;
  explicit Other(const std::wstring& str) : curr_str(str) {}
  void operator()() const {};
};
}  // namespace states

namespace events {
struct Letter { wchar_t symbol; void operator()() const {}; };
struct DecimalDigit { wchar_t symbol; void operator()() const {}; };
struct AllBasesDigit { wchar_t symbol; void operator()() const {}; };
struct BaseSeparator { wchar_t symbol; void operator()() const {}; };
struct Dot { wchar_t symbol; void operator()() const {}; };
struct Space { wchar_t symbol; void operator()() const {}; };
struct Curly { wchar_t symbol; void operator()() const {}; };
struct Parentheses { wchar_t symbol; void operator()() const {}; };
struct Quotes { wchar_t symbol; void operator()() const {}; };
struct Underline { wchar_t symbol; void operator()() const {}; };
struct Operation { wchar_t symbol; void operator()() const {}; };
struct Semicolor { wchar_t symbol; void operator()() const {}; };
}  // namespace events

using  State = std::variant<states::Begin,
                            states::RTI,
                            states::Literal,
                            states::StringLiteral,
                            states::SpecialSymbols,
                            states::Other>;

using Event = std::variant<events::Letter,
                           events::DecimalDigit,
                           events::AllBasesDigit,
                           events::BaseSeparator,
                           events::Dot,
                           events::Space,
                           events::Curly,
                           events::Parentheses,
                           events::Quotes,
                           events::Underline,
                           events::Operation,
                           events::Semicolor>;

// Я не ебу, что делает этот код
template <class... Ts>
struct overload : Ts...
{
  using Ts::operator()...;
};

template <class... Ts>
overload(Ts...) -> overload<Ts...>;
// Конец, дальше ебу

class FiniteStateMachine {
 public:
  FiniteStateMachine(const wchar_t* programm_text, size_t programm_lenght);

  std::vector<Token> getTokens();

  ~FiniteStateMachine() = default;

 private:
  const wchar_t* program_text_;
  size_t program_lenght_;
  std::vector<Token> tokens_;
  bool stop_in_symbol_ = false;
  const wchar_t* curr_symbol_;
  size_t curr_line_ = 1;
  Trie reserved_words_;
  Trie type_words_;

  void startProcessText();

  Event getEvent(const wchar_t symbol) const;

  void operations(const std::wstring& str);

  void applyState(states::Other state);
  void applyState(states::SpecialSymbols state);
  void applyState(states::RTI state);
  void applyState(states::Literal state);
  void applyState(states::StringLiteral state);
  // Блять, Артём, я конечно всё понимаю, но пиши, пожалуйста свои комментарии
  // так, что они не кидали Typo

  // Ааахxахахa, ещe чегo мнe делaть
  void applyState(State state);  // Кидвет искоючение

  // Извините конечно, но этот пиздец надо было сделать
  // Begin
  State onEvent(states::Begin state, events::Space event);
  State onEvent(states::Begin state, events::Curly event);
  State onEvent(states::Begin state, events::Parentheses event);
  State onEvent(states::Begin state, events::Quotes event);
  State onEvent(states::Begin state, events::DecimalDigit event);
  State onEvent(states::Begin state, events::BaseSeparator event);
  State onEvent(states::Begin state, events::AllBasesDigit event);
  State onEvent(states::Begin state, events::Letter event);
  State onEvent(states::Begin state, events::Operation event);
  State onEvent(states::Begin state, Event event);

  // StringLiteral
  State onEvent(states::StringLiteral state, events::Quotes event);
  State onEvent(states::StringLiteral state, Event event);

  // Literal
  State onEvent(states::Literal state, events::Dot event);
  State onEvent(states::Literal state, events::BaseSeparator event);
  State onEvent(states::Literal state, events::AllBasesDigit event);
  State onEvent(states::Literal state, events::DecimalDigit event);
  State onEvent(states::Literal state, Event event);

  // RTI
  State onEvent(states::RTI state, events::Dot event);
  State onEvent(states::RTI state, events::Letter event);
  State onEvent(states::RTI state, events::BaseSeparator event);
  State onEvent(states::RTI state, events::AllBasesDigit event);
  State onEvent(states::RTI state, events::DecimalDigit event);
  State onEvent(states::RTI state, events::Underline event);
  State onEvent(states::RTI state, Event event);

  // Operation
  State onEvent(states::SpecialSymbols state, events::Dot event);
  State onEvent(states::SpecialSymbols state, events::Operation event);
  State onEvent(states::SpecialSymbols state, Event event);

  // Other
  State onEvent(states::Other state, Event event);

  // Default
  State onEvent(State state, Event event);
};
}  // namespace lexer
