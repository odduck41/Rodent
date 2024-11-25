#include "common.hpp"
#include "parser.hpp"
#include "Test.h"
#include "TF.hpp"
#include <iostream>

// Лол, если basic.hpp - первый include, то оно нахуй падает, хотя везде есть #pragma once...
#include "basic.hpp"
// #include "FSM.hpp"
// #include <exceptions.hpp>

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

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
  } catch (undeclared& err) {
    std::wcerr << err.what(0);
    return -1;
  } catch (redeclaration& err) {
    std::wcerr << err.what(0);
    return -1;
  }
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 10);
#else
    std::cout << "\e[32m";
#endif // _WIN32
    std::cout << "OK";
  return 0;
}
