#pragma once

#include <cstddef>
#include <string>

struct Trie {
  Trie* children[26]{};
  bool terminal{};
};

enum Lexeme {
  reserved,       // Зарезервированные символы
  identifier,     // Идентификатор
  literal,        // Литерал
  type,           // Тип
  s_literal,      // Строковый литерал
  operation,      // Оператор
  punctuation,    // Пунктуация
  dot,            // Точка
  comma,          // Запятая
  parentheses     // Круглые скобки
};

struct Token {
  Lexeme type{};
  const char* content{};
  size_t line{};
};

extern Trie* reserved_;
extern Trie* types_;

void load_reserved();
void load_basic_types();

void add(Trie*&, char);
void addAll(const char*, Trie*);

bool inTrie(const char*, size_t = 0, const Trie* = reserved_);

std::string tokenize(const char* from);
