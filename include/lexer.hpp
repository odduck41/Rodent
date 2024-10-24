#pragma once

#include <cstddef>

struct Trie {
  Trie* children[26]{};
  bool terminal{};
};

enum Lexeme {
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
  Lexeme type{};
  char* content{};
  size_t line{};
};

extern Trie* reserved_;

void load();
void add(Trie*&, char);
void addAll(const char*);
bool inTrie(const char*, size_t = 0, const Trie* = reserved_);
void tokenize();
