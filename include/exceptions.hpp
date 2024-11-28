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
    explicit bad_lexeme(const Token& t, const std::wstring& filename_) {
        message_ = L"Bad lexeme at line ";
        message_ += std::to_wstring(t.line);
        message_ += L" in file ";
        message_ += filename_;
        message_ += L":\n";
        message_ += t.content;
        message_ += L"\t type: ";
        message_ += asWstring(t.type);
    };
    [[nodiscard]] const wchar_t* what(int) const noexcept {
        return message_.c_str();
    }
private:
  std::wstring message_;
};

class redeclaration final : public std::exception {
public:
    explicit redeclaration(const Token& t) {
        message_ += L"Redeclaration of variable ";
        message_ += t.content;
        message_ += L" at line ";
        message_ += std::to_wstring(t.line);
    };
    [[nodiscard]] const wchar_t* what(int) const noexcept {
        return message_.c_str();
    }
private:
    std::wstring message_;
};

class undefined final : public std::exception {
public:
    explicit undefined(const Token& t) {
        message_ += L"Using of undefined variable ";
        message_ += t.content;
        message_ += L" at line ";
        message_ += std::to_wstring(t.line);
    }

    explicit undefined(const Token& t, int) {
        message_ += L"Using of undefined function ";
        message_ += t.content;
        message_ += L" at line ";
        message_ += std::to_wstring(t.line);
    }

    [[nodiscard]] const wchar_t* what(int) const noexcept {
        return message_.c_str();
    }
private:
    std::wstring message_;
};

class redefinition final : public std::exception {
public:
    explicit redefinition(const Token& t) {
        message_ += L"Redefinition of function ";
        message_ += t.content;
        message_ += L" at line ";
        message_ += std::to_wstring(t.line);
    };
    [[nodiscard]] const wchar_t* what(int) const noexcept {
        return message_.c_str();
    }
private:
    std::wstring message_;
};
