#include <malloc.h>

#ifndef RODENT_LEXER_H
#define RODENT_LEXER_H

#define bool char
#define true 1
#define false 0

#define new(T) (T*)malloc(sizeof(T))
#define delete(T) free(T)

struct Trie {
  struct Trie* children[26];
  bool terminal;
};

enum Type {
  reserved,
  identifier,
  literal,
  s_literal,
  operation,
  punctuation,
  point,
  comma,
  brackets
};

struct token {
  enum Type type;
  char* content;
  int line;
};

void load();
void tokenize();

#endif //RODENT_LEXER_H
