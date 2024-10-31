#include "FSM.h"
#include <cwctype>
#include <optional>

std::vector<Token> tokens;

size_t line_ = 1;

void add(std::wstring& token, const Lexeme& type) {
    tokens.push_back({type, token, line_});
    token.clear();
}

std::optional<Lexeme> bracket(const wchar_t& symbol) {
    if (symbol == '{') return Lexeme::OCurly;
    if (symbol == '}') return Lexeme::CCurly;
    if (symbol == '(') return Lexeme::OParentheses;
    if (symbol == ')') return Lexeme::CParentheses;
    if (symbol == '[') return Lexeme::OSquare;
    if (symbol == ']') return Lexeme::CSquare;
    return std::nullopt;
}

void stateMachine(const wchar_t* program) {
    std::wstring token;
    auto state_ = State::S;
    size_t index = 0;
    while (program[index] != '\0') {

        if (program[index] == '\r') continue;
        if (program[index] == '\n') {
            ++line_;
            ++index;
            continue;
        }


        switch (state_) {
            case State::S:
                if (isalpha(program[index])) {
                    state_ = State::RTI;
                    break;
                }

                if (isalnum(program[index])) {
                    state_ = State::Literal;
                    break;
                }

                if (auto type_ = bracket(program[index]); type_ != std::nullopt) {
                    add(token, type_.value());
                }

                break;
            case State::RTI:
            case State::Dot:
            case State::Literal:
            case State::SLiteral:
            case State::EoSLiteral:
            case State::Other:
                break;
        }

        token += program[index];
        ++index;
    }
}
