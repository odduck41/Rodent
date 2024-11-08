#pragma once
#include <string>
#include "files.hpp"

namespace lexer {
inline void removeComments(size_t, wchar_t*&);

class Trie {
 public:
  explicit Trie(const char* filename);

  bool check(const std::wstring&) const;

 private:
  struct Node {
    Node* children[26]{};
    bool terminal = false;
  };

  Node* root = new Node;

  void process(RFile& file, const long long size_);

  void addAll(const wchar_t*);

  void add(wchar_t, Node*);
};
}  // namespace lexer
