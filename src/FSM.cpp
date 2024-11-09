#include "FSM.hpp"

#include "exceptions.hpp"

#include <codecvt>
#include <cstdint>
#include <cstdio>
#include <cwctype>
#include <locale>
#include <optional>

namespace lexer {
FiniteStateMachine::FiniteStateMachine(const char* programm_text,
                                       size_t programm_lenght)
    : reserved_words_("../assets/reserved.txt"),
      type_words_("../assets/types.txt") {}

void FiniteStateMachine::operations(const std::wstring& str) {}

void FiniteStateMachine::applyState(states::SpecialSymbols state) {
  operations(state.curr_str);
}

void FiniteStateMachine::applyState(states::RTI state) {
  if (reserved_words_.check(state.curr_str)) {
    tokens_.push_back({Lexeme::Reserved, state.curr_str, curr_line_});
  } else if (type_words_.check(state.curr_str)) {
    tokens_.push_back({Lexeme::Type, state.curr_str, curr_line_});
  } else {
    tokens_.push_back({Lexeme::Identifier, state.curr_str, curr_line_});
  }
}

void FiniteStateMachine::applyState(states::Literal state) {
  enum NumberBases { Binary, Decimal, Hexadecimal } curr_number_base;
  std::wstring curr_number_wstring;
  if (state.curr_str.length() >= 3) {
    if (state.curr_str[1] == 'b') {
      curr_number_base = NumberBases::Binary;
    } else if (state.curr_str[1] == 'x') {
      curr_number_base = NumberBases::Hexadecimal;
    } else {
      curr_number_base = NumberBases::Decimal;
    }
  } else {
    curr_number_base = NumberBases::Decimal;
  }

  if (curr_number_base == Decimal) {
    uint64_t number = stoi(state.curr_str);
    char curr_number_char_arr[17];
    sprintf(curr_number_char_arr, "%016X", number);
    std::string curr_number_string_arr(curr_number_char_arr);

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    curr_number_wstring = converter.from_bytes(curr_number_string_arr);
  } else if (curr_number_base == Binary) {
    uint64_t number = stoi(state.curr_str.substr(2));
    char curr_number_char_arr[17];
    sprintf(curr_number_char_arr, "%016X", number);
    std::string curr_number_string_arr(curr_number_char_arr);

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    curr_number_wstring = converter.from_bytes(curr_number_string_arr);
  } else if (curr_number_base == Hexadecimal) {
    curr_number_wstring = state.curr_str.substr(2);
  }

  tokens_.push_back({Lexeme::Literal, curr_number_wstring, curr_line_});
}

void FiniteStateMachine::applyState(states::StringLiteral state) {
  tokens_.push_back({Lexeme::StringLiteral, state.curr_str, curr_line_});
}

void FiniteStateMachine::applyState(states::Other state) {
  tokens_.push_back({Lexeme::Other, state.curr_str, curr_line_});
}

void FiniteStateMachine::applyState(states::State state) {
  throw lexer_error("Default applyState() has called");
}

std::vector<Token> FiniteStateMachine::getTokens() {
  return tokens_;
}
}  // namespace lexer