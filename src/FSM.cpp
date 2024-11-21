#include "FSM.hpp"

#include "exceptions.hpp"

#include <codecvt>
#include <cstdint>
#include <cstdio>
// #include <cwctype>
#include <locale>
// #include <optional>
// #include <utility>

namespace lexer {
FiniteStateMachine::FiniteStateMachine(const wchar_t* program_text,
                                       const size_t program_length)
    : program_text_(program_text),
      program_length_(program_length),
      reserved_words_("../assets/reserved.txt"),
      type_words_("../assets/types.txt") {
  startProcessText();
}

void FiniteStateMachine::operations(const std::wstring& str) {
  for (std::vector<Token> new_tokens = parseOperations(str, curr_line_); Token& new_token : new_tokens) {
    tokens_.push_back(new_token);
  }
}

void FiniteStateMachine::applyState(const states::SpecialSymbols &state) {
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
    std::wstring curr_ctr;
    for (const wchar_t& symbol : state.curr_str) {
      if (symbol == ';') {
        operations(curr_ctr);
        curr_ctr = L"";
        tokens_.push_back({Lexeme::Semicolon, L";", curr_line_});
      } else if (symbol == ',') {
        operations(curr_ctr);
        curr_ctr = L"";
        tokens_.push_back({Lexeme::Punctuation, L",", curr_line_});
      } else {
        curr_ctr += symbol;
      }
    }
    if (!curr_ctr.empty()) {
      operations(state.curr_str);
    }
  }
}

void FiniteStateMachine::applyState(const states::RTI& state) {
  if (reserved_words_.check(state.curr_str)) {
    tokens_.push_back({Lexeme::Reserved, state.curr_str, curr_line_});
  } else if (type_words_.check(state.curr_str) || (state.curr_str[state.curr_str.length() - 1] == L'&' && type_words_.check(state.curr_str.substr(0, state.curr_str.length() - 1)))) {
    tokens_.push_back({Lexeme::Type, state.curr_str, curr_line_});
  } else {
    tokens_.push_back({Lexeme::Identifier, state.curr_str, curr_line_});
  }
}

void FiniteStateMachine::applyState(const states::Literal &state) {
  enum NumberBases { Binary, Decimal, DecimalFloat, Hexadecimal, Other } curr_number_base;
  std::wstring curr_number_wstring;
  if (state.curr_str.length() >= 3) {
    if (state.curr_str[1] == 'b') {
      for (int i = 2; i < state.curr_str.length(); ++i) {
        if (state.curr_str[i] != '0' && state.curr_str[i] != '1') {
          curr_number_base = NumberBases::Other;
          goto skip_bases_check;
        }
      }
      curr_number_base = NumberBases::Binary;
    } else if (state.curr_str[1] == 'x') {
      for (int i = 2; i < state.curr_str.length(); ++i) {
        if (!(state.curr_str[i] >= '0' && state.curr_str[i] <= '9') &&
            !(state.curr_str[i] >= 'a' && state.curr_str[i] <= 'f') &&
            !(state.curr_str[i] >= 'A' && state.curr_str[i] <= 'F')) {
          curr_number_base = NumberBases::Other;
          goto skip_bases_check;
        }
      }
      curr_number_base = NumberBases::Hexadecimal;
    } else {
      for (int i = 0; i < state.curr_str.length(); ++i) {
        if ((state.curr_str[i] < '0' || state.curr_str[i] > '9') && state.curr_str[i] != '.') {
          curr_number_base = NumberBases::Other;
          goto skip_bases_check;
        }
      }
      for (int i = 0; i < state.curr_str.length(); ++i) {
        if (state.curr_str[i] == '.') {
          curr_number_base = NumberBases::DecimalFloat;
          goto skip_bases_check;
        }
      }
      curr_number_base = NumberBases::Decimal;
    }
  } else {
    for (int i = 0; i < state.curr_str.length(); ++i) {
      if ((state.curr_str[i] < '0' || state.curr_str[i] > '9') && state.curr_str[i] != '.') {
        curr_number_base = NumberBases::Other;
        goto skip_bases_check;
      }
    }
    for (int i = 0; i < state.curr_str.length(); ++i) {
      if (state.curr_str[i] == '.') {
        curr_number_base = NumberBases::DecimalFloat;
        goto skip_bases_check;
      }
    }
    curr_number_base = NumberBases::Decimal;
  }
  skip_bases_check:

  if (curr_number_base == Other) {
    curr_number_wstring = state.curr_str;
    tokens_.push_back({Lexeme::Other, curr_number_wstring, curr_line_});
    return;
  } else if (curr_number_base == Decimal) {
    const uint64_t number = stoi(state.curr_str);
    char curr_number_char_arr[17];
    for (char& i : curr_number_char_arr) {
      i = 0;
    }
    sprintf(curr_number_char_arr, "%llX", number);
    const std::string curr_number_string_arr(curr_number_char_arr);

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    curr_number_wstring = converter.from_bytes(curr_number_string_arr);
  } else if (curr_number_base == DecimalFloat) {
    const double number_double = stod(state.curr_str);
    const auto number = std::bit_cast<const uint64_t>(number_double);
    char curr_number_char_arr[17];
    for (char& i : curr_number_char_arr) {
      i = 0;
    }
    sprintf(curr_number_char_arr, "%llX", number);
    const std::string curr_number_string_arr(curr_number_char_arr);

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    curr_number_wstring = converter.from_bytes(curr_number_string_arr);
  } else if (curr_number_base == Binary) {
    const std::wstring number_str = state.curr_str.substr(2);
    uint64_t number = 0;
    for (int i = 0; i < number_str.length(); ++i) {
      if (number_str[number_str.length() - i - 1] == L'1') {
        number += (1 << i);
      }
    }
    char curr_number_char_arr[17];
    for (char& i : curr_number_char_arr) {
      i = 0;
    }
    sprintf(curr_number_char_arr, "%llX", number);
    const std::string curr_number_string_arr(curr_number_char_arr);

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    curr_number_wstring = converter.from_bytes(curr_number_string_arr);
  } else {
    curr_number_wstring = state.curr_str.substr(2);
  }

  tokens_.push_back({Lexeme::Literal, curr_number_wstring, curr_line_});
}

void FiniteStateMachine::applyState(const states::StringLiteral& state) {
  tokens_.push_back({Lexeme::StringLiteral, state.curr_str, curr_line_});
}

void FiniteStateMachine::applyState(const states::Other& state) {
  tokens_.push_back({Lexeme::Other, state.curr_str, curr_line_});
}

void FiniteStateMachine::applyState(const State& state) {
  throw lexer_error("Default applyState() has called");
}

Event FiniteStateMachine::getEvent(const wchar_t symbol) {
  if (symbol == '.') {
    events::Dot new_event{};
    new_event.symbol = symbol;
    return new_event;
  }
  if (symbol == ' ') {
    events::Space new_event{};
    new_event.symbol = symbol;
    return new_event;
  }
  if (symbol == '_') {
    events::Underline new_event{};
    new_event.symbol = symbol;
    return new_event;
  }
  if (symbol == '{' || symbol == '}') {
    events::Curly new_event{};
    new_event.symbol = symbol;
    return new_event;
  }
  if (symbol == '"') {
    events::Quotes new_event{};
    new_event.symbol = symbol;
    return new_event;
  }
  if (symbol == '(' || symbol == ')') {
    events::Parentheses new_event{};
    new_event.symbol = symbol;
    return new_event;
  }
  if (symbol == '+' || symbol == '-' || symbol == '=' ||
             symbol == '!' || symbol == '%' || symbol == '*' || symbol == '/' ||
             symbol == '^' || symbol == '|' || symbol == '&' || symbol == '<' ||
             symbol == '>' || symbol == ',' || symbol == '[' ||
             symbol == ']' || symbol == ';' || symbol == ':') {
    events::Operation new_event{};
    new_event.symbol = symbol;
    return new_event;
  }
  if (symbol == 'x' || symbol == 'b') {
    events::BaseSeparator new_event{};
    new_event.symbol = symbol;
    return new_event;
  }
  if (symbol >= '0' && symbol <= '9') {
    events::DecimalDigit new_event{};
    new_event.symbol = symbol;
    return new_event;
  }
  if ((symbol >= '0' && symbol <= '9') ||
             (symbol >= 'a' && symbol <= 'f') ||
             (symbol >= 'A' && symbol <= 'F')) {
    events::AllBasesDigit new_event{};
    new_event.symbol = symbol;
    return new_event;
  }
  events::Letter new_event{};
  new_event.symbol = symbol;
  return new_event;
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
  if (std::holds_alternative<states::SpecialSymbols>(curr_state))  {
    applyState(std::get<states::SpecialSymbols>(curr_state));
  }
}

// Begin
State FiniteStateMachine::onEvent(const states::Begin& state,
                                          events::Space event) {
  return states::Begin{};
}

State FiniteStateMachine::onEvent(const states::Begin& state,
                                  const events::Curly event) {
  applyState(states::SpecialSymbols(std::wstring(1, event.symbol)));
  return states::Begin{};
}

State FiniteStateMachine::onEvent(const states::Begin& state,
                                  const events::Parentheses event) {
  applyState(states::SpecialSymbols(std::wstring(1, event.symbol)));
  return states::Begin{};
}

State FiniteStateMachine::onEvent(const states::Begin& state,
                                          events::Quotes event) {
  return states::StringLiteral{};
}

State FiniteStateMachine::onEvent(const states::Begin& state,
                                  const events::DecimalDigit event) {
  return states::Literal{std::wstring(1, event.symbol)};
}

State FiniteStateMachine::onEvent(const states::Begin& state,
                                  const events::BaseSeparator event) {
  return states::RTI{std::wstring(1, event.symbol)};
}

State FiniteStateMachine::onEvent(const states::Begin& state,
                                  const events::Letter event) {
  return states::RTI{std::wstring(1, event.symbol)};
}

State FiniteStateMachine::onEvent(const states::Begin& state,
                                  const events::AllBasesDigit event) {
  return states::RTI{std::wstring(1, event.symbol)};
}

State FiniteStateMachine::onEvent(const states::Begin& state,
                                  const events::Operation event) {
  return states::SpecialSymbols{std::wstring(1, event.symbol)};
}

State FiniteStateMachine::onEvent(const states::Begin& state,
                                          Event event) const {
  return states::Other{std::wstring(1, *curr_symbol_)};
}

// StringLiteral
State FiniteStateMachine::onEvent(const states::StringLiteral& state,
                                          events::Quotes event) {
  applyState(state);
  return states::Begin{};
}

State FiniteStateMachine::onEvent(const states::StringLiteral& state,
                                          Event event) const {
  return states::StringLiteral{state.curr_str + std::wstring(1, *curr_symbol_)};
}

// Literal
State FiniteStateMachine::onEvent(const states::Literal& state,
                                  const events::Dot event) {
  return states::Literal{state.curr_str + std::wstring(1, event.symbol)};
}

State FiniteStateMachine::onEvent(const states::Literal& state,
                                  const events::BaseSeparator event) {
  return states::Literal{state.curr_str + std::wstring(1, event.symbol)};
}

State FiniteStateMachine::onEvent(const states::Literal& state,
                                  const events::AllBasesDigit event) {
  return states::Literal{state.curr_str + std::wstring(1, event.symbol)};
}

State FiniteStateMachine::onEvent(const states::Literal& state,
                                  const events::DecimalDigit event) {
  return states::Literal{state.curr_str + std::wstring(1, event.symbol)};
}

State FiniteStateMachine::onEvent(const states::Literal& state,
                                  [[maybe_unused]] Event event) {
  applyState(state);
  stop_in_symbol_ = true;
  return states::Begin{};
}

// RTI
State FiniteStateMachine::onEvent(const states::RTI& state,
                                  const events::Dot event) {
  return states::RTI{state.curr_str + std::wstring(1, event.symbol)};
}

State FiniteStateMachine::onEvent(const states::RTI& state,
                                  const events::Letter event) {
  return states::RTI{state.curr_str + std::wstring(1, event.symbol)};
}

State FiniteStateMachine::onEvent(const states::RTI& state,
                                  const events::AllBasesDigit event) {
  return states::RTI{state.curr_str + std::wstring(1, event.symbol)};
}

State FiniteStateMachine::onEvent(const states::RTI& state,
                                  const events::BaseSeparator event) {
  return states::RTI{state.curr_str + std::wstring(1, event.symbol)};
}

State FiniteStateMachine::onEvent(const states::RTI& state,
                                  const events::DecimalDigit event) {
  return states::RTI{state.curr_str + std::wstring(1, event.symbol)};
}

  State FiniteStateMachine::onEvent(const states::RTI& state,
                                    const events::Operation event) {
  if (event.symbol == L'&') {
    return states::RTI{state.curr_str + std::wstring(1, event.symbol)};
  }
  applyState(state);
  stop_in_symbol_ = true;
  return states::Begin{};
}

State FiniteStateMachine::onEvent(const states::RTI& state,
                                  const events::Underline event) {
  return states::RTI{state.curr_str + std::wstring(1, event.symbol)};
}

State FiniteStateMachine::onEvent(const states::RTI& state,
                                  [[maybe_unused]] Event event) {
  applyState(state);
  stop_in_symbol_ = true;
  return states::Begin{};
}

// Operation
State FiniteStateMachine::onEvent(const states::SpecialSymbols& state,
                                  const events::Dot event) {
  return states::SpecialSymbols(state.curr_str + std::wstring(1, event.symbol));
}

State FiniteStateMachine::onEvent(const states::SpecialSymbols& state,
                                  const events::Operation event) {
  return states::SpecialSymbols(state.curr_str + std::wstring(1, event.symbol));
}

State FiniteStateMachine::onEvent(const states::SpecialSymbols& state,
                                  [[maybe_unused]] Event event) {
  applyState(state);
  stop_in_symbol_ = true;
  return states::Begin{};
}

// Other
State FiniteStateMachine::onEvent(const states::Other& state,
                                  [[maybe_unused]] Event event) {
  applyState(state);
  return states::Other{};
}

[[maybe_unused]] State FiniteStateMachine::onEvent(const State& state, Event event) { return {}; };

// Default
// State FiniteStateMachine::onEvent(State state,
//                                           Event event) {
//   throw lexer_error("Default onEvent() has called");
// }

std::vector<Token> FiniteStateMachine::getTokens() {
  return tokens_;
}
}  // namespace lexer
