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


class bad_lexeme final : public std::exception {
 public:
    explicit bad_lexeme(const Token& t) {
        message_ = L"Bad lexeme at line ";
        message_ += std::to_wstring(t.line);
        message_ += L":\n";
        message_ += t.content;
    };
    [[nodiscard]] const wchar_t* what(int) const noexcept {
        return message_.c_str();
    }
private:
  std::wstring message_;
};