#pragma once

#include <exception>
#include <string>
#include <vector>
#include "TF.hpp"
#include "basic.hpp"
#include "validator.hpp"

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
    explicit undeclared_function(const std::wstring& name, const size_t line, const std::vector<TF::FunctionArgument>& args) {
        message_ = L"Undeclared function: ";
        message_ += name;
        message_ += L"(";
        for (int i = 0; i < args.size(); ++i) {
            message_ += args[i].type;
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

class bad_operator final : std::exception {
public:
    explicit bad_operator(const Variable* a, const Element* op, const Variable* b) {
        if (op == nullptr) {
            message_ = L"Bad binary operator between ";
            message_ += a->unit.content;
            message_ += L" and ";
            message_ += b->unit.content;
            message_ += L" operands at line ";
            message_ += std::to_wstring(a->unit.line);
            return;
        }
        message_ += L"Bad operation ";
        message_ += op->unit.content;
        message_ += L" between ";
        message_ += a->unit.content;
        message_ += L" and ";
        message_ += b->unit.content;
        message_ += L" operands at line ";
        message_ += std::to_wstring(op->unit.line);
    }

    explicit bad_operator(const Variable* a, const Variable* op) {
        if (op == nullptr) {
            message_ = L"Bad unary operator near ";
            message_ += a->unit.content;
            message_ += L" operand at line ";
            message_ += std::to_wstring(a->unit.line);
            return;
        }
        message_ = L"Bad unary operator near ";
        message_ += a->unit.content;
        message_ += L" operand at line ";
        message_ += std::to_wstring(op->unit.line);
    }

    [[nodiscard]] const wchar_t* what(int) const noexcept {
        return message_.c_str();
    }

private:
    std::wstring message_{};
};

class wrong_operands final : std::exception {
public:
    explicit wrong_operands(const Variable* a, const Variable* b) {
        message_ = L"Incongruous types of ";
        message_ += a->unit.content;
        message_ += L" and ";
        message_ += b->unit.content;
        message_ += L" at line ";
        message_ += std::to_wstring(a->unit.line);
    }
    explicit wrong_operands(const Variable* a) {
        message_ += L"Bad type of operand ";
        message_ += a->unit.content;
        message_ += L" at line ";
        message_ += std::to_wstring(a->unit.line);
    }
    [[nodiscard]] const wchar_t* what(int) const noexcept {
        return message_.c_str();
    }
private:
    std::wstring message_;
};