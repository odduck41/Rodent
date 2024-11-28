#pragma once

#include <exception>
#include <string>
#include "basic.hpp"
#include "semantic.hpp"

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

class bad_operator final : public std::exception {
public:
    explicit bad_operator(const Element* a, const Element* op, const Element* b) {
        message_ += L"Expected, that ";
        message_ += a->content;
        message_ += L" is a value, ";
        message_ += op->content;
        message_ += L" is an operation and ";
        message_ += b->content;
        message_ += L" is also a value, but they don't. Line: ";
        message_ += std::to_wstring(op->line);
    }

    [[nodiscard]] const wchar_t* what(int) const noexcept {
        return message_.c_str();
    }
private:
    std::wstring message_;
};

class bad_value final : public std::exception {
public:
    explicit bad_value(const Element* a) {
        message_ += L"Expected, that ";
        message_ += a->content;
        message_ += L" is a lvalue, but it doesn't. Line: ";
        message_ += std::to_wstring(a->line);
    }

    [[nodiscard]] const wchar_t* what(int) const noexcept {
        return message_.c_str();
    }
private:
    std::wstring message_;
};

class wrong_operands final : std::exception {
public:
    explicit wrong_operands(const Element* a, const Element* b) {
        message_ = L"Incongruous types of ";
        message_ += a->content;
        message_ += L" and ";
        message_ += b->content;
        message_ += L" at line ";
        message_ += std::to_wstring(a->line);
    }
    explicit wrong_operands(const Element* a) {
        message_ += L"Bad type of operand ";
        message_ += a->content;
        message_ += L" at line ";
        message_ += std::to_wstring(a->line);
    }
    [[nodiscard]] const wchar_t* what(int) const noexcept {
        return message_.c_str();
    }
private:
    std::wstring message_;
};

class bad_type final : public std::exception {
public:
    explicit bad_type(const Token& t) {
        message_ += L"Wrong type of variable ";
        message_ += t.content;
        message_ += L" at line ";
        message_ += std::to_wstring(t.line);
    }
    explicit bad_type(const Type& t, const size_t line) {
        message_ += L"Wrong type of variable ";
        message_ += t;
        message_ += L" at line ";
        message_ += std::to_wstring(line);
    };
    [[nodiscard]] const wchar_t* what(int) const noexcept {
        return message_.c_str();
    }
private:
    std::wstring message_;
};

class bad_return final : public std::exception {
public:
    explicit bad_return(const Type& a, const Type& b, const size_t& line) {
        message_ += L"Return type ";
        message_ += a;
        message_ += L" at line ";
        message_ += std::to_wstring(line);
        message_ += L"is not coming down to the function type ";
        message_ += b;
    };
    [[nodiscard]] const wchar_t* what(int) const noexcept {
        return message_.c_str();
    }
private:
    std::wstring message_;
};