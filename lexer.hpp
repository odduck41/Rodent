#pragma once

#ifndef RODENT_LEXER_H
#define RODENT_LEXER_H

#include <cstddef>

struct Trie {
  struct Trie* children[26];
  bool terminal;
};

enum Type {
  reserved,       // Зарезервированные символы
  identifier,     // Идентификатор
  literal,        // Литерал
  s_literal,      // Строковый литерал
  operation,      // Оператор
  punctuation,    // Пунктуация
  point,          // Точка
  comma,          // Запятая
  parentheses     // Круглые скобки
};

struct Token {
  enum Type type{};
  char* content{};
  size_t line{};
};

extern Trie* reserved_;

void add();
void load();
void tokenize();

#endif //RODENT_LEXER_H
