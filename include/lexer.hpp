#pragma once
#include <string>

struct Trie {
  Trie* children[26]{};
  bool terminal = false;
};

extern Trie* reserved;
extern Trie* types;

void removeComments(size_t, char*&);

void loadReserved(const char* = "../reserved.txt");
void loadTypes(const char* = "../types.txt");

void addAll(const char*, Trie*);
void add(char, Trie*&);

bool inTrie(const char*, const Trie*, size_t = 0);
bool inTrie(const std::string&, const Trie*, size_t = 0);

