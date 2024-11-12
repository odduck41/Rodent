#pragma once

#include <map>

#include <iostream>
#include "FSM.hpp"
#include "exceptions.hpp"
#include "files.hpp"
#include "parser.hpp"
#include "lexer.hpp"
// #include "test.h"

template <class... T>
std::map<wchar_t, void (*)(T...)> function;

inline std::string filename;
inline std::string where;
inline std::string out;

inline void lex(const char* output = "a.dev") {
  out = output;
}

static void defineFlags() {
  function<const char*>['l'] = &lex;
}

inline void split() {
    std::string res;
    for (long long i = static_cast<long long>(filename.size()) - 1ll; i >= 0; --i) {
        if (filename[i] == '\\') {
            where = filename.substr(0, i + 1);
            break;
        }
        std::string nw;
        nw += filename[i];
        res = (nw += res);
    }
    filename = res;

}

inline void l(const std::vector<Token>& tokens) {
    WFile os(out.c_str());
    os << L"{\n";
    for (size_t i = 0; i < tokens.size(); ++i) {
      os << L"\tToken{" << asWstring(tokens[i].type) << L", \"" << tokens[i].content << L"\", " << tokens[i].line << L"}";
      if (i != tokens.size() - 1) {
          os << L",";
      }
      os << L"\n";
    }
    os << L"}\n";
}

static void doAll(const std::string& filename = filename) {
    // loadReserved();
    // loadTypes();

  RFile file((where + filename).c_str());
  const long long size_ = file.size();

  auto program = new wchar_t[size_ + 1];

  file.read(program, size_);

  program[size_] = '\0';

  lexer::removeComments(size_ + 1, program);

  lexer::FiniteStateMachine fsm(program, size_);
  std::vector<Token> tokens = fsm.getTokens();

  if (!out.empty()) {
    l(tokens);
  }

  Parser p(tokens, filename);

  delete[] program;
}

inline void doFlags(const int argc, const char** argv) {
  defineFlags();
  for (size_t i = 1; i < argc; ++i) {
    if (argv[i][0] == '-') {
      if (function<const char*>.contains(argv[i][1])) {
        if (i + 1 != argc && argv[i + 1][0] != '-') {
          function<const char*>[argv[i][1]](argv[i + 1]);
        }
        ++i;
        goto next;
      }
      throw bad_flag(argv[i]);
    }
    filename = argv[i];

  next:;
  }
  split();
  doAll();
}
