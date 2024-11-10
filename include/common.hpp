#pragma once

#include <map>

#include <iostream>
#include "FSM.hpp"
#include "exceptions.hpp"
#include "files.hpp"
#include "lexer.hpp"
// #include "test.h"

template <class... T>
std::map<wchar_t, void (*)(T...)> function;

inline std::string filename;
inline std::string out;

inline void lex(const char* output = "a.dev") {
  out = output;
}

static void defineFlags() {
  function<const char*>['l'] = &lex;
}

static void doAll(const std::string& filename = filename) {
    // loadReserved();
    // loadTypes();

  RFile file(filename.c_str());
  const long long size_ = file.size();

  auto program = new wchar_t[size_ + 1];

  file.read(program, size_);

  program[size_] = '\0';

  lexer::removeComments(size_ + 1, program);

  lexer::FiniteStateMachine fsm(program, size_);
  std::vector<Token> tokens = fsm.getTokens();

  for (Token token : tokens) {
    std::cout << token.line << ": ";
    std::wcout << token.content;
    std::cout << " | ";
    if (token.type == Lexeme::Reserved) {
      std::cout << "Reserved" << '\n';
    }
    if (token.type == Lexeme::Identifier) {
      std::cout << "Identifier" << '\n';
    }
    if (token.type == Lexeme::Type) {
      std::cout << "Type" << '\n';
    }
    if (token.type == Lexeme::Literal) {
      std::cout << "Literal" << '\n';
    }
    if (token.type == Lexeme::StringLiteral) {
      std::cout << "StringLiteral" << '\n';
    }
    if (token.type == Lexeme::Operation) {
      std::cout << "Operation" << '\n';
    }
    if (token.type == Lexeme::Punctuation) {
      std::cout << "Punctuation" << '\n';
    }
    if (token.type == Lexeme::Semicolon) {
      std::cout << "Semicolon" << '\n';
    }
    if (token.type == Lexeme::OpenParentheses) {
      std::cout << "OpenParentheses" << '\n';
    }
    if (token.type == Lexeme::CloseParentheses) {
      std::cout << "CloseParentheses" << '\n';
    }
    if (token.type == Lexeme::OpenCurly) {
      std::cout << "OpenCurly" << '\n';
    }
    if (token.type == Lexeme::CloseCurly) {
      std::cout << "CloseCurly" << '\n';
    }
    if (token.type == Lexeme::Other) {
      std::cout << "Other" << '\n';
    }
  }

  delete[] program;
}

inline void doFlags(int argc, const char** argv) {
  defineFlags();
  for (size_t i = 1; i < argc; ++i) {
    if (argv[i][0] == '-') {
      if (function<>.contains(argv[i][1]) ||
          function<const wchar_t*>.contains(argv[i][1])) {
        if (i + 1 != argc && argv[i + 1][0] != '-') {
          function<const wchar_t*>[argv[i][1]](
              reinterpret_cast<const wchar_t*>(argv[i + 1]));
        } else {
          function<>[argv[i][1]]();
        }

        goto next;
      }
      throw bad_flag(argv[i]);
    }
    filename = argv[i];

  next:;
  }

  doAll();
}
