#include "common.hpp"
#include "parser.hpp"
#include "Test.h"
#include <iostream>

// Лол, если basic.hpp - первый include, то оно нахуй падает, хотя везде есть #pragma once...
#include "basic.hpp"
// #include "FSM.hpp"
// #include <exceptions.hpp>

int main(int argc, const char** argv) {
  // Trie();
  // operations();

  // doFlags(argc, argv);
  // Trie();
  // operations();

  try {
    doFlags(argc, argv);
  } catch (bad_lexeme& err) {
    std::wcerr << err.what(0);
    return -1;
  }
  // HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  // SetConsoleTextAttribute(hConsole, 10);
  printf("OK");
  return 0;
}