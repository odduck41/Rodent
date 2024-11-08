#include <cstddef>
#include <string>

#include "files.hpp"
#include "lexer.hpp"

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

void Trie::process(RFile& file, const long long size_) {
  const auto str = new wchar_t[size_ + 1];
  file.read(str, size_);
  str[size_] = '\0';

  addAll(str);

  delete[] str;
}

void Trie::addAll(const wchar_t* str) {
  Trie* current = root;
  for (size_t i = 0; str[i] != '\0'; ++i) {
    if (str[i] == '\n' || str[i] == '\r' || str[i] == ' ') {
      current->terminal = true;
      current = trie;
    } else {
      add(str[i], current);
    }
  }
  current->terminal = true;
}

void Trie::add(const wchar_t s, Node* curr_node) {
  if (curr_node->children[s - 'a'] == nullptr) {
    curr_node->children[s - 'a'] = new Trie;
  }
  curr_node = curr_node->children[s - 'a'];
}

bool Trie::check(const std::wstring& s) const {
  size_t sz = s.size();
  if (sz == s.size() && this->root->terminal) {
    return true;
  }
  if (sz == s.size()) {
    return false;
  }

  if (this->root->children[s[sz] - 'a'] == nullptr)
    return false;
  return inTrie(s, this->root->children[s[sz] - 'a'], sz + 1);
}

}  // namespace lexer