#pragma once

#include <exception>
#include <string>
#include "basic.hpp"

class bad_flag final : public std::exception {
 public:
  explicit bad_flag(const char* data) { message_ += data; }

  [[nodiscard]] const char* what() const noexcept override {
    return message_.c_str();
  }

 private:
  std::string message_{"Incorrect flag: "};
};

class lexer_error final : public std::exception {
 public:
  explicit lexer_error(const char* data) : message_(data) {}

  [[nodiscard]] const char* what() const noexcept override {
    return message_.c_str();
  }

 private:
  std::string message_;
};

inline const std::wstring& operator+= (std::wstring& s, const Lexeme& l) {
    if (l == Lexeme::Reserved) s += L"Reserved";
    else if (l == Lexeme::Identifier) s += L"Identifier";
    else if (l == Lexeme::Type) s += L"Type";
    else if (l == Lexeme::Literal) s += L"Literal";
    else if (l == Lexeme::StringLiteral) s += L"StringLiteral";
    else if (l == Lexeme::Operation) s += L"Operation";
    else if (l == Lexeme::Punctuation) s += L"Punctuation";
    else if (l == Lexeme::Semicolon) s += L"Semicolon";
    else if (l == Lexeme::OpenParentheses) s += L"Open Parentheses";
    else if (l == Lexeme::CloseParentheses) s += L"Close Parentheses";
    else if (l == Lexeme::OpenCurly) s += L"Open Curly";
    else if (l == Lexeme::CloseCurly) s += L"Close Curly";
    else s += L"Other";

    return s;
}

class bad_lexeme final : public std::exception {
 public:
    explicit bad_lexeme(const Token& t, const std::wstring& filename_) {
        message_ = L"Bad lexeme at line ";
        message_ += std::to_wstring(t.line);
        message_ += L" in file ";
        message_ += filename_;
        message_ += L":\n";
        message_ += t.content;
        message_ += L"\t type: ";
        message_ += t.type;
    };
    [[nodiscard]] const wchar_t* what(int) const noexcept {
        return message_.c_str();
    }
private:
  std::wstring message_;
};