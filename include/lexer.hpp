#pragma once
#include <string>

#include "basic.hpp"
#include "files.hpp"
#include <vector>

namespace lexer {

inline void removeComments(size_t, wchar_t*&);


class Trie {
 public:
  explicit Trie(const char* filename);

  [[nodiscard]] bool check(const std::wstring&) const;

 private:
  struct Node {
    Node* children[26]{};
    bool terminal = false;
  };

  Node* root = new Node;

  void process(RFile& file, long long size_) const;

  void addAll(const wchar_t*) const;

  static void add(wchar_t, Node*&);

  friend bool inTrie(const wchar_t* s, const Trie::Node* trie, size_t sz);
  friend bool inTrie(const std::wstring& s, const Trie::Node* trie, size_t sz);
};
}  // namespace lexer

std::vector<Token> parseOperations(const std::wstring & seq, size_t line);