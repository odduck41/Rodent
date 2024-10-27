#pragma once
#include <string>

struct Trie {
  Trie* children[26]{};
  bool terminal = false;
};

extern Trie* reserved;
extern Trie* types;

void removeComments(size_t, wchar_t*&);

void loadReserved(const char* = "../reserved.txt");
void loadTypes(const char* = "../types.txt");

void addAll(const wchar_t*, Trie*);
void add(wchar_t, Trie*&);

bool inTrie(const wchar_t*, const Trie*, size_t = 0);
bool inTrie(const std::wstring&, const Trie*, size_t = 0);

