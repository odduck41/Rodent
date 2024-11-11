#pragma once

#include "lexer.hpp"
#include "basic.hpp"

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <variant>

namespace lexer {

namespace states {
struct Begin {
  std::wstring curr_str;
  Begin() = default;
  explicit Begin(std::wstring  str) : curr_str(std::move(str)) {}
  void operator()() const {};
};
struct RTI {
  std::wstring curr_str;
  RTI() = default;
  explicit RTI(std::wstring  str) : curr_str(std::move(str)) {}
  void operator()() const {};
};
struct Literal {
  std::wstring curr_str;
  Literal() = default;
  explicit Literal(std::wstring  str) : curr_str(std::move(str)) {}
  void operator()() const {};
};
struct StringLiteral {
  std::wstring curr_str;
  StringLiteral() = default;
  explicit StringLiteral(std::wstring  str) : curr_str(std::move(str)) {}
  void operator()() const {};
};
struct SpecialSymbols {
  std::wstring curr_str;
  SpecialSymbols() = default;
  explicit SpecialSymbols(std::wstring  str) : curr_str(std::move(str)) {}
  void operator()() const {};
};
struct Other {
  std::wstring curr_str;
  Other() = default;
  explicit Other(std::wstring  str) : curr_str(std::move(str)) {}
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
struct Semicolon { wchar_t symbol; void operator()() const {}; };
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
                           events::Semicolon>;

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
  FiniteStateMachine(const wchar_t* program_text, size_t program_length);

  std::vector<Token> getTokens();

  ~FiniteStateMachine() = default;

 private:
  const wchar_t* program_text_;
  size_t program_length_;
  std::vector<Token> tokens_{};
  bool stop_in_symbol_ = false;
  const wchar_t* curr_symbol_{};
  size_t curr_line_ = 1;
  Trie reserved_words_{};
  Trie type_words_{};

  void startProcessText();

  [[nodiscard]] static Event getEvent(wchar_t symbol) ;

  void operations(const std::wstring& str);

  void applyState(const states::Other& state);
  void applyState(const states::SpecialSymbols &state);
  void applyState(const states::RTI& state);
  void applyState(const states::Literal &state);
  void applyState(const states::StringLiteral& state);
  // Блять, Артём, я конечно всё понимаю, но пиши, пожалуйста свои комментарии
  // так, что они не кидали Typo

  // Ааахxахахa, ещe чегo мнe делaть
  static void applyState(const State& state);  // Кидвет искоючение

  // Извините конечно, но этот пиздец надо было сделать
  // Begin
  static State onEvent(const states::Begin& state, events::Space event);
  State onEvent(const states::Begin& state, events::Curly event);
  State onEvent(const states::Begin& state, events::Parentheses event);
  static State onEvent(const states::Begin& state, events::Quotes event);

  static State onEvent(const states::Begin& state, events::DecimalDigit event);
  static State onEvent(const states::Begin& state, events::BaseSeparator event);
  static State onEvent(const states::Begin& state, events::AllBasesDigit event);
  static State onEvent(const states::Begin& state, events::Letter event);
  static State onEvent(const states::Begin& state, events::Operation event);
  [[nodiscard]] State onEvent(const states::Begin& state, Event event) const;

  // StringLiteral
  State onEvent(const states::StringLiteral& state, events::Quotes event);
  [[nodiscard]] State onEvent(const states::StringLiteral& state, Event event) const;

  // Literal
  static State onEvent(const states::Literal& state, events::Dot event);
  static State onEvent(const states::Literal& state, events::BaseSeparator event);
  static State onEvent(const states::Literal& state, events::AllBasesDigit event);
  static State onEvent(const states::Literal& state, events::DecimalDigit event);
  State onEvent(const states::Literal& state, Event event);

  // RTI
  static State onEvent(const states::RTI& state, events::Dot event);
  static State onEvent(const states::RTI& state, events::Letter event);
  static State onEvent(const states::RTI& state, events::BaseSeparator event);
  static State onEvent(const states::RTI& state, events::AllBasesDigit event);
  static State onEvent(const states::RTI& state, events::DecimalDigit event);
  static State onEvent(const states::RTI& state, events::Underline event);
  State onEvent(const states::RTI& state, Event event);

  // Operation
  static State onEvent(const states::SpecialSymbols& state, events::Dot event);
  static State onEvent(const states::SpecialSymbols& state, events::Operation event);
  State onEvent(const states::SpecialSymbols& state, Event event);

  // Other
  State onEvent(const states::Other& state, Event event);

  // Default
  static State onEvent(const State& state, Event event);
};
}  // namespace lexer
