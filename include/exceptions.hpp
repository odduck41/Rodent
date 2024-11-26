#pragma once

#include <exception>
#include <string>
#include <vector>
#include <iostream>
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
    explicit redeclaration(const std::wstring& name, const size_t line1, const size_t line2) {
        message_ = L"Redefinition of variable ";
        message_ += name;
        message_ += L" at line ";
        message_ += std::to_wstring(line2);
        message_ += L", first definition at line ";
        message_ += std::to_wstring(line1);
    }
    [[nodiscard]] const wchar_t* what(int) const noexcept {
        return message_.c_str();
    }
private:
    std::wstring message_;
};

class undeclared final : public std::exception {
public:
    explicit undeclared(const std::wstring& name, const size_t line) {
        message_ = L"Undeclared variable: ";
        message_ += name;
        message_ += L" at line ";
        message_ += std::to_wstring(line);
    }
    [[nodiscard]] const wchar_t* what(int) const noexcept {
        return message_.c_str();
    }
private:
    std::wstring message_;
};

class undeclared_function final : public std::exception {
public:
    explicit undeclared_function(const std::wstring& name, const size_t line, const std::vector<std::wstring>& args) {
        message_ = L"Undeclared function: ";
        message_ += name;
        message_ += L"(";
        for (int i = 0; i < args.size(); ++i) {
            message_ += args[i];
            if (i != args.size() - 1) {
                message_ += L", ";
            }
        }
        message_ += L")";
        message_ += L" at line ";
        message_ += std::to_wstring(line);
    }
    [[nodiscard]] const wchar_t* what(int) const noexcept {
        return message_.c_str();
    }
private:
    std::wstring message_;
};

class redeclaration_function final : public std::exception {
public:
    explicit redeclaration_function(const std::wstring& name, const std::vector<std::wstring>& args, const size_t line1, const size_t line2) {
        message_ = L"Redefinition of function ";
        message_ += name;
        message_ += L"(";
        for (int i = 0; i < args.size(); ++i) {
            message_ += args[i];
            if (i != args.size() - 1) {
                message_ += L", ";
            }
        }
        message_ += L")";
        message_ += L" at line ";
        message_ += std::to_wstring(line2);
        message_ += L", first definition at line ";
        message_ += std::to_wstring(line1);
    }
    [[nodiscard]] const wchar_t* what(int) const noexcept {
        return message_.c_str();
    }
private:
    std::wstring message_;
};