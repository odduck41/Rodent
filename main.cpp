#include "common.hpp"
#include "parser.hpp"
#include "Test.h"
// #include "FSM.hpp"
// #include <exceptions.hpp>

int main(int argc, const char** argv) {

  // doFlags(argc, argv);
  // Trie();
  // operations();
  Parser({Token{Lexeme::Reserved, L"func", 1},
    {}, {}, {}, {}, {}, {}});
  return 0;
}