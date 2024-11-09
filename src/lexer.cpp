#include <cstddef>
#include <string>

#include "files.hpp"
#include "lexer.hpp"

#include <utility>

namespace lexer {
inline void removeComments(const size_t sz, wchar_t*& program) {
    std::wstring result;
    result.reserve(sz + 1);

    bool single{}, multiline{};
    for (size_t i = 0; program[i] != '\0'; ++i) {
      if (program[i] == '\r') {
        continue;
      }

      if (single) {
        if (program[i] == '\n') {
          single = false;
        } else {
          continue;
        }
      }

      if (multiline) {
        if (program[i] == '\n') {
          result += program[i];
          continue;
        }

        if (program[i] == '*' && program[i + 1] == '/') {
          multiline = false;
          ++i;
        }

        continue;
      }

      if (program[i] == '/' && program[i + 1] == '/') {
        single = true;
        continue;
      }

      if (program[i] == '/' && program[i + 1] == '*') {
        multiline = true;
        continue;
      }

      result += program[i];
    }

    result.copy(program, result.size());
    program[result.size()] = '\0';
}

Trie::Trie(const char* filename) {
  RFile file(filename);

  process(file, file.size());
}

bool Trie::check(const wchar_t* s) const {
    return check_(s, root);
}

bool Trie::check(const std::wstring& s) const {
    return check_(s, root);
}

void Trie::process(RFile& file, const long long size_) const {
  const auto str = new wchar_t[size_ + 1];
  file.read(str, size_);
  str[size_] = '\0';

  addAll(str);

  delete[] str;
}

void Trie::addAll(const wchar_t* str) const {
  Node* current = root;
  for (size_t i = 0; str[i] != '\0'; ++i) {
    if (str[i] == '\n' || str[i] == '\r' || str[i] == ' ') {
      current->terminal = true;
      current = root;
    } else {
      add(str[i], current);
    }
  }
  current->terminal = true;
}

void Trie::add(const wchar_t s, Node*& curr_node) {
  if (curr_node->children[s - 'a'] == nullptr) {
    curr_node->children[s - 'a'] = new Node;
  }
  curr_node = curr_node->children[s - 'a'];
}


bool Trie::check_(const wchar_t* s, const Trie::Node* trie, const size_t sz) {
    if (s[sz] == '\0' && trie->terminal) {
      return true;
    }
    if (s[sz] == '\0') {
      return false;
    }

    if (trie->children[s[sz] - 'a'] == nullptr) return false;
    return check_(s, trie->children[s[sz] - 'a'], sz + 1);
}

bool Trie::check_(const std::wstring& s, const Trie::Node* trie, const size_t sz) {
    if (sz == s.size() && trie->terminal) {
      return true;
    }
    if (sz == s.size()) {
      return false;
    }

    if (trie->children[s[sz] - 'a'] == nullptr) return false;
    return check_(s, trie->children[s[sz] - 'a'], sz + 1);
}

}  // namespace lexer


std::vector<Token> parseOperations(const std::wstring & seq, size_t line) {
    std::vector<Token> tokens;
    tokens.reserve(seq.size());

    size_t symbol = 0;
    std::wstring operation;
    auto adUn = [&symbol, &operation, &tokens, &seq = std::as_const(seq), &line = std::as_const(line)]() {
      // &, |, +, -

      operation += seq[symbol];

      if (symbol + 1 == seq.size()) goto exit;

      ++symbol;
      if (seq[symbol] == '=' || seq[symbol] == operation[0] || (operation[0] == '-' && seq[symbol] == '>')) {
        operation += seq[symbol];
      } else --symbol;

      exit: tokens.push_back({Lexeme::Operation, operation, line});
    };

    auto muEq = [&symbol, &operation, &tokens, &seq = std::as_const(seq), &line = std::as_const(line)]() {
      // умножение и прочее
      operation += seq[symbol];

      if (symbol + 1 == seq.size()) goto exit;

      ++symbol;
      if (seq[symbol] == '=') {
        operation += seq[symbol];
      } else --symbol;

      exit: tokens.push_back({Lexeme::Operation, operation, line});
    };

    auto comparing = [&symbol, &operation, &tokens, &seq = std::as_const(seq), &line = std::as_const(line)]() {
      operation += seq[symbol];

      if (symbol + 1 == seq.size()) goto exit;

      ++symbol;
      if (seq[symbol] == '=') {
        operation += seq[symbol];
      } else if (seq[symbol] == operation[0]) {
        operation += seq[symbol];

        if (symbol + 1 == seq.size()) goto exit;

        ++symbol;

        if (seq[symbol] == '=') {
          operation += seq[symbol];
        } else --symbol;

      } else --symbol;

      exit: tokens.push_back({Lexeme::Operation, operation, line});
    };

    while (symbol != seq.size()) {
      if (seq[symbol] == '+' || seq[symbol] == '-' || seq[symbol] == '&' || seq[symbol] == '|') adUn();
      else if (seq[symbol] == '*' || seq[symbol] == '/' || seq[symbol] == '!' || seq[symbol] == '=') muEq();
      else comparing();

      operation.clear();
      ++symbol;
    }

    return tokens;
}
