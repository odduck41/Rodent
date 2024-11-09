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

  bool check(const wchar_t*) const;

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

  static bool check_(const wchar_t*, const Trie::Node*, size_t = 0);

  static bool check_(const std::wstring&, const Trie::Node*, size_t = 0);
};
}  // namespace lexer

std::vector<Token> parseOperations(const std::wstring & seq, size_t line);