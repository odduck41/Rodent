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
  reserved,       // Зарезервированные символы
  identifier,     // Идентификатор
  literal,        // Литерал
  s_literal,      // Строковый литерал
  operation,      // Опрератор
  punctuation,    // Пунктуация
  point,          // Точка
  comma,          // Запятая
  parentheses     // Круглые скобки
};

struct Token {
  enum Type type;
  char* content;
  size_t line;
};

void load();
void tokenize();

#endif //RODENT_LEXER_H
