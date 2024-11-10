#include "FSM.hpp"

#include "exceptions.hpp"

#include <codecvt>
#include <cstdint>
#include <cstdio>
#include <cwctype>
#include <locale>
#include <optional>

namespace lexer {
FiniteStateMachine::FiniteStateMachine(const wchar_t* program_text,
                                       size_t program_lenght)
    : reserved_words_("../assets/reserved.txt"),
      type_words_("../assets/types.txt"),
      program_text_(program_text),
      program_lenght_(program_lenght) {
  startProcessText();
}

void FiniteStateMachine::operations(const std::wstring& str) {
  std::vector<Token> new_tokens = parseOperations(str, curr_line_);

  for (Token& new_token : new_tokens) {
    tokens_.push_back(new_token);
  }
}

void FiniteStateMachine::applyState(states::SpecialSymbols* state) {
  if (state->curr_str.length() == 1) {
    if (state->curr_str[0] == static_cast<wchar_t>('{')) {
      tokens_.push_back({Lexeme::OpenCurly, state->curr_str, curr_line_});
    } else if (state->curr_str[0] == static_cast<wchar_t>('}')) {
      tokens_.push_back({Lexeme::CloseCurly, state->curr_str, curr_line_});
    } else if (state->curr_str[0] == static_cast<wchar_t>('(')) {
      tokens_.push_back({Lexeme::OpenParentheses, state->curr_str, curr_line_});
    } else if (state->curr_str[0] == static_cast<wchar_t>(')')) {
      tokens_.push_back({Lexeme::CloseParentheses, state->curr_str, curr_line_});
    } else if (state->curr_str[0] == static_cast<wchar_t>(',')) {
      tokens_.push_back({Lexeme::Punctuation, state->curr_str, curr_line_});
    } else if (state->curr_str[0] == static_cast<wchar_t>(';')) {
      tokens_.push_back({Lexeme::Semicolon, state->curr_str, curr_line_});
    } else {
      operations(state->curr_str);
    }
  } else {
    operations(state->curr_str);
  }
}

void FiniteStateMachine::applyState(states::RTI* state) {
  if (reserved_words_.check(state->curr_str)) {
    tokens_.push_back({Lexeme::Reserved, state->curr_str, curr_line_});
  } else if (type_words_.check(state->curr_str)) {
    tokens_.push_back({Lexeme::Type, state->curr_str, curr_line_});
  } else {
    tokens_.push_back({Lexeme::Identifier, state->curr_str, curr_line_});
  }
}

void FiniteStateMachine::applyState(states::Literal* state) {
  enum NumberBases { Binary, Decimal, Hexadecimal } curr_number_base;
  std::wstring curr_number_wstring;
  if (state->curr_str.length() >= 3) {
    if (state->curr_str[1] == 'b') {
      curr_number_base = NumberBases::Binary;
    } else if (state->curr_str[1] == 'x') {
      curr_number_base = NumberBases::Hexadecimal;
    } else {
      curr_number_base = NumberBases::Decimal;
    }
  } else {
    curr_number_base = NumberBases::Decimal;
  }

  if (curr_number_base == Decimal) {
    uint64_t number = stoi(state->curr_str);
    char curr_number_char_arr[17];
    sprintf(curr_number_char_arr, "%016X", number);
    std::string curr_number_string_arr(curr_number_char_arr);

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    curr_number_wstring = converter.from_bytes(curr_number_string_arr);
  } else if (curr_number_base == Binary) {
    uint64_t number = stoi(state->curr_str.substr(2));
    char curr_number_char_arr[17];
    sprintf(curr_number_char_arr, "%016X", number);
    std::string curr_number_string_arr(curr_number_char_arr);

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    curr_number_wstring = converter.from_bytes(curr_number_string_arr);
  } else if (curr_number_base == Hexadecimal) {
    curr_number_wstring = state->curr_str.substr(2);
  }

  tokens_.push_back({Lexeme::Literal, curr_number_wstring, curr_line_});
}

void FiniteStateMachine::applyState(states::StringLiteral* state) {
  tokens_.push_back({Lexeme::StringLiteral, state->curr_str, curr_line_});
}

void FiniteStateMachine::applyState(states::Other* state) {
  tokens_.push_back({Lexeme::Other, state->curr_str, curr_line_});
}

void FiniteStateMachine::applyState(states::State* state) {
  throw lexer_error("Default applyState() has called");
}

events::Event* FiniteStateMachine::getEvent(const wchar_t symbol) const {
  if (symbol == '.') {
    auto* new_event = new events::Dot;
    new_event->symbol = symbol;
    return new_event;
  } else if (symbol == ' ') {
    auto* new_event = new events::Space;
    new_event->symbol = symbol;
    return new_event;
  } else if (symbol == '_') {
    auto* new_event = new events::Underline;
    new_event->symbol = symbol;
    return new_event;
  } else if (symbol == '{' || symbol == '}') {
    auto* new_event = new events::Curly;
    new_event->symbol = symbol;
    return new_event;
  } else if (symbol == '"') {
    auto* new_event = new events::Quotes;
    new_event->symbol = symbol;
    return new_event;
  } else if (symbol == '(' || symbol == ')') {
    auto* new_event = new events::Parentheses;
    new_event->symbol = symbol;
    return new_event;
  } else if (symbol == '+' || symbol == '+' || symbol == '-' || symbol == '=' ||
             symbol == '!' || symbol == '%' || symbol == '*' || symbol == '/' ||
             symbol == '^' || symbol == '|' || symbol == '&' || symbol == '<' ||
             symbol == '>' || symbol == ',' || symbol == '.' || symbol == '[' ||
             symbol == ']') {
    auto* new_event = new events::Operation;
    new_event->symbol = symbol;
    return new_event;
  } else if (symbol == 'x' || symbol == 'b') {
    auto* new_event = new events::BaseSeparator;
    new_event->symbol = symbol;
    return new_event;
  } else if (symbol >= '0' && symbol <= '9') {
    auto* new_event = new events::DecimalDigit;
    new_event->symbol = symbol;
    return new_event;
  } else if ((symbol >= '0' && symbol <= '9') ||
             (symbol >= 'a' && symbol <= 'f') ||
             (symbol >= 'A' && symbol <= 'F')) {
    auto* new_event = new events::AllBasesDigit;
    new_event->symbol = symbol;
    return new_event;
  } else {
    auto* new_event = new events::Letter;
    new_event->symbol = symbol;
    return new_event;
  }
}

void FiniteStateMachine::startProcessText() {
  const wchar_t* curr_symbol = program_text_;
  states::State* curr_state = new states::Begin{};

  while (*curr_symbol) {
    if (*curr_symbol == '\n') {
      ++curr_line_;
      ++curr_symbol;
      continue;
    }
    if (*curr_symbol == '\r') {
      ++curr_symbol;
      continue;
    }
    events::Event* curr_event = getEvent(*curr_symbol);
    curr_state = onEvent(curr_state, curr_event);
    if (curr_state->is_begin() && curr_state->curr_str.length() != 0) {
      curr_state = new states::Begin{};
      continue;
    }
    ++curr_symbol;
  }
}

// Begin
states::State* FiniteStateMachine::onEvent(states::Begin* state,
                                          events::Space* event) {
  return new states::Begin{};
}

states::State* FiniteStateMachine::onEvent(states::Begin* state,
                                          events::Curly* event) {
  applyState(new states::SpecialSymbols(std::wstring(1, event->symbol)));
  return new states::Begin{};
}

states::State* FiniteStateMachine::onEvent(states::Begin* state,
                                          events::Parentheses* event) {
  applyState(new states::SpecialSymbols(std::wstring(1, event->symbol)));
  return new states::Begin{};
}

states::State* FiniteStateMachine::onEvent(states::Begin* state,
                                          events::Quotes* event) {
  return new states::StringLiteral{};
}

states::State* FiniteStateMachine::onEvent(states::Begin* state,
                                          events::DecimalDigit* event) {
  return new states::Literal{std::wstring(1, event->symbol)};
}

states::State* FiniteStateMachine::onEvent(states::Begin* state,
                                          events::Letter* event) {
  return new states::RTI{std::wstring(1, event->symbol)};
}

states::State* FiniteStateMachine::onEvent(states::Begin* state,
                                          events::Operation* event) {
  return new states::SpecialSymbols{std::wstring(1, event->symbol)};
}

states::State* FiniteStateMachine::onEvent(states::Begin* state,
                                          events::Event* event) {
  return new states::Other{std::wstring(1, event->symbol)};
}

// StringLiteral
states::State* FiniteStateMachine::onEvent(states::StringLiteral* state,
                                          events::Quotes* event) {
  applyState(state);
  return new states::Begin{};
}

states::State* FiniteStateMachine::onEvent(states::StringLiteral* state,
                                          events::Event* event) {
  return new states::StringLiteral{state->curr_str + std::wstring(1, event->symbol)};
}

// Literal
states::State* FiniteStateMachine::onEvent(states::Literal* state,
                                          events::Dot* event) {
  return new states::Literal{state->curr_str + std::wstring(1, event->symbol)};
}

states::State* FiniteStateMachine::onEvent(states::Literal* state,
                                          events::BaseSeparator* event) {
  return new states::Literal{state->curr_str + std::wstring(1, event->symbol)};
}

states::State* FiniteStateMachine::onEvent(states::Literal* state,
                                          events::AllBasesDigit* event) {
  return new states::Literal{state->curr_str + std::wstring(1, event->symbol)};
}

states::State* FiniteStateMachine::onEvent(states::Literal* state,
                                          events::Event* event) {
  applyState(state);
  return new states::Begin{std::wstring(1, event->symbol)};
}

// RTI
states::State* FiniteStateMachine::onEvent(states::RTI* state,
                                          events::Dot* event) {
  return new states::RTI{state->curr_str + std::wstring(1, event->symbol)};
}

states::State* FiniteStateMachine::onEvent(states::RTI* state,
                                          events::Letter* event) {
  return new states::RTI{state->curr_str + std::wstring(1, event->symbol)};
}

states::State* FiniteStateMachine::onEvent(states::RTI* state,
                                          events::DecimalDigit* event) {
  return new states::RTI{state->curr_str + std::wstring(1, event->symbol)};
}

states::State* FiniteStateMachine::onEvent(states::RTI* state,
                                          events::Underline* event) {
  return new states::RTI{state->curr_str + std::wstring(1, event->symbol)};
}

states::State* FiniteStateMachine::onEvent(states::RTI* state,
                                          events::Event* event) {
  applyState(state);
  return new states::Begin{std::wstring(1, event->symbol)};
}

// Operation
states::State* FiniteStateMachine::onEvent(states::SpecialSymbols* state,
                                          events::Dot* event) {
  return new states::SpecialSymbols(state->curr_str + std::wstring(1, event->symbol));
}

states::State* FiniteStateMachine::onEvent(states::SpecialSymbols* state,
                                          events::Operation* event) {
  return new states::SpecialSymbols(state->curr_str + std::wstring(1, event->symbol));
}

states::State* FiniteStateMachine::onEvent(states::SpecialSymbols* state,
                                          events::Event* event) {
  applyState(state);
  return new states::Begin(std::wstring(1, event->symbol));
}

// Other
states::State* FiniteStateMachine::onEvent(states::Other* state,
                                          events::Event* event) {
  return new states::Other{state->curr_str + std::wstring(1, event->symbol)};
}

// Default
// states::State* FiniteStateMachine::onEvent(states::State* state,
//                                           events::Event* event) {
//   throw lexer_error("Default onEvent() has called");
// }

std::vector<Token> FiniteStateMachine::getTokens() {
  return tokens_;
}
}  // namespace lexer