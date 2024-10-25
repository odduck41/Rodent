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
  std::string content;
  size_t line{};
};

extern Trie* reserved_;
extern Trie* types_;

void load_reserved();
void load_basic_types();

const char* removeComments(const char* line);

void add(Trie*&, char);
void addAll(const char*, Trie*);

bool inTrie(const char*, const Trie*, size_t = 0);

std::string tokenize(const char* from);
