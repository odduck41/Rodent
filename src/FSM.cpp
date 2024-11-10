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

void FiniteStateMachine::applyState(states::SpecialSymbols state) {
  if (state.curr_str.length() == 1) {
    if (state.curr_str[0] == static_cast<wchar_t>('{')) {
      tokens_.push_back({Lexeme::OpenCurly, state.curr_str, curr_line_});
    } else if (state.curr_str[0] == static_cast<wchar_t>('}')) {
      tokens_.push_back({Lexeme::CloseCurly, state.curr_str, curr_line_});
    } else if (state.curr_str[0] == static_cast<wchar_t>('(')) {
      tokens_.push_back({Lexeme::OpenParentheses, state.curr_str, curr_line_});
    } else if (state.curr_str[0] == static_cast<wchar_t>(')')) {
      tokens_.push_back({Lexeme::CloseParentheses, state.curr_str, curr_line_});
    } else if (state.curr_str[0] == static_cast<wchar_t>(',')) {
      tokens_.push_back({Lexeme::Punctuation, state.curr_str, curr_line_});
    } else if (state.curr_str[0] == static_cast<wchar_t>(';')) {
      tokens_.push_back({Lexeme::Semicolon, state.curr_str, curr_line_});
    } else {
      operations(state.curr_str);
    }
  } else {
    operations(state.curr_str);
  }
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

void FiniteStateMachine::applyState(State state) {
  throw lexer_error("Default applyState() has called");
}

Event FiniteStateMachine::getEvent(const wchar_t symbol) const {
  if (symbol == '.') {
    events::Dot new_event;
    new_event.symbol = symbol;
    return new_event;
  } else if (symbol == ' ') {
    events::Space new_event;
    new_event.symbol = symbol;
    return new_event;
  } else if (symbol == '_') {
    events::Underline new_event;
    new_event.symbol = symbol;
    return new_event;
  } else if (symbol == '{' || symbol == '}') {
    events::Curly new_event;
    new_event.symbol = symbol;
    return new_event;
  } else if (symbol == '"') {
    events::Quotes new_event;
    new_event.symbol = symbol;
    return new_event;
  } else if (symbol == '(' || symbol == ')') {
    events::Parentheses new_event;
    new_event.symbol = symbol;
    return new_event;
  } else if (symbol == '+' || symbol == '+' || symbol == '-' || symbol == '=' ||
             symbol == '!' || symbol == '%' || symbol == '*' || symbol == '/' ||
             symbol == '^' || symbol == '|' || symbol == '&' || symbol == '<' ||
             symbol == '>' || symbol == ',' || symbol == '.' || symbol == '[' ||
             symbol == ']' || symbol == ';' || symbol == ':') {
    events::Operation new_event;
    new_event.symbol = symbol;
    return new_event;
  } else if (symbol == 'x' || symbol == 'b') {
    events::BaseSeparator new_event;
    new_event.symbol = symbol;
    return new_event;
  } else if (symbol >= '0' && symbol <= '9') {
    events::DecimalDigit new_event;
    new_event.symbol = symbol;
    return new_event;
  } else if ((symbol >= '0' && symbol <= '9') ||
             (symbol >= 'a' && symbol <= 'f') ||
             (symbol >= 'A' && symbol <= 'F')) {
    events::AllBasesDigit new_event;
    new_event.symbol = symbol;
    return new_event;
  } else {
    events::Letter new_event;
    new_event.symbol = symbol;
    return new_event;
  }
}

void FiniteStateMachine::startProcessText() {
  const wchar_t* curr_symbol = program_text_;
  curr_symbol_ = curr_symbol;
  State curr_state = states::Begin{};

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
    Event curr_event = getEvent(*curr_symbol);
    curr_symbol_ = curr_symbol;

    std::visit(
        overload{
            [this, &curr_state](auto const& state_, auto const& event_)
            {
                curr_state = onEvent(state_, event_);
            }
        },
        curr_state, curr_event
    );


    if (stop_in_symbol_) {
      stop_in_symbol_ = false;
    } else {
      ++curr_symbol;
    }
  }
}

// Begin
State FiniteStateMachine::onEvent(states::Begin state,
                                          events::Space event) {
  return states::Begin{};
}

State FiniteStateMachine::onEvent(states::Begin state,
                                          events::Curly event) {
  applyState(states::SpecialSymbols(std::wstring(1, event.symbol)));
  return states::Begin{};
}

State FiniteStateMachine::onEvent(states::Begin state,
                                          events::Parentheses event) {
  applyState(states::SpecialSymbols(std::wstring(1, event.symbol)));
  return states::Begin{};
}

State FiniteStateMachine::onEvent(states::Begin state,
                                          events::Quotes event) {
  return states::StringLiteral{};
}

State FiniteStateMachine::onEvent(states::Begin state,
                                          events::DecimalDigit event) {
  return states::Literal{std::wstring(1, event.symbol)};
}

State FiniteStateMachine::onEvent(states::Begin state,
                                              events::BaseSeparator event) {
  return states::RTI{std::wstring(1, event.symbol)};
}

State FiniteStateMachine::onEvent(states::Begin state,
                                              events::Letter event) {
  return states::RTI{std::wstring(1, event.symbol)};
}

State FiniteStateMachine::onEvent(states::Begin state,
                                            events::AllBasesDigit event) {
  return states::RTI{std::wstring(1, event.symbol)};
}

State FiniteStateMachine::onEvent(states::Begin state,
                                          events::Operation event) {
  return states::SpecialSymbols{std::wstring(1, event.symbol)};
}

State FiniteStateMachine::onEvent(states::Begin state,
                                          Event event) {
  return states::Other{std::wstring(1, *curr_symbol_)};
}

// StringLiteral
State FiniteStateMachine::onEvent(states::StringLiteral state,
                                          events::Quotes event) {
  applyState(state);
  return states::Begin{};
}

State FiniteStateMachine::onEvent(states::StringLiteral state,
                                          Event event) {
  return states::StringLiteral{state.curr_str + std::wstring(1, *curr_symbol_)};
}

// Literal
State FiniteStateMachine::onEvent(states::Literal state,
                                          events::Dot event) {
  return states::Literal{state.curr_str + std::wstring(1, event.symbol)};
}

State FiniteStateMachine::onEvent(states::Literal state,
                                          events::BaseSeparator event) {
  return states::Literal{state.curr_str + std::wstring(1, event.symbol)};
}

State FiniteStateMachine::onEvent(states::Literal state,
                                            events::AllBasesDigit event) {
  return states::Literal{state.curr_str + std::wstring(1, event.symbol)};
}

State FiniteStateMachine::onEvent(states::Literal state,
                                            events::DecimalDigit event) {
  return states::Literal{state.curr_str + std::wstring(1, event.symbol)};
}

State FiniteStateMachine::onEvent(states::Literal state,
                                          Event event) {
  applyState(state);
  stop_in_symbol_ = true;
  return states::Begin{};
}

// RTI
State FiniteStateMachine::onEvent(states::RTI state,
                                          events::Dot event) {
  return states::RTI{state.curr_str + std::wstring(1, event.symbol)};
}

State FiniteStateMachine::onEvent(states::RTI state,
                                            events::Letter event) {
  return states::RTI{state.curr_str + std::wstring(1, event.symbol)};
}

State FiniteStateMachine::onEvent(states::RTI state,
                                            events::AllBasesDigit event) {
  return states::RTI{state.curr_str + std::wstring(1, event.symbol)};
}

State FiniteStateMachine::onEvent(states::RTI state,
                                            events::BaseSeparator event) {
  return states::RTI{state.curr_str + std::wstring(1, event.symbol)};
}

State FiniteStateMachine::onEvent(states::RTI state,
                                          events::DecimalDigit event) {
  return states::RTI{state.curr_str + std::wstring(1, event.symbol)};
}

State FiniteStateMachine::onEvent(states::RTI state,
                                          events::Underline event) {
  return states::RTI{state.curr_str + std::wstring(1, event.symbol)};
}

State FiniteStateMachine::onEvent(states::RTI state,
                                          Event event) {
  applyState(state);
  stop_in_symbol_ = true;
  return states::Begin{};
}

// Operation
State FiniteStateMachine::onEvent(states::SpecialSymbols state,
                                          events::Dot event) {
  return states::SpecialSymbols(state.curr_str + std::wstring(1, event.symbol));
}

State FiniteStateMachine::onEvent(states::SpecialSymbols state,
                                          events::Operation event) {
  return states::SpecialSymbols(state.curr_str + std::wstring(1, event.symbol));
}

State FiniteStateMachine::onEvent(states::SpecialSymbols state,
                                          Event event) {
  applyState(state);
  stop_in_symbol_ = true;
  return states::Begin{};
}

// Other
State FiniteStateMachine::onEvent(states::Other state,
                                          Event event) {
  applyState(state);
  return states::Other{};
}

// Default
// State FiniteStateMachine::onEvent(State state,
//                                           Event event) {
//   throw lexer_error("Default onEvent() has called");
// }

std::vector<Token> FiniteStateMachine::getTokens() {
  return tokens_;
}
}  // namespace lexer