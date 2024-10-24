#pragma once
#include <cstddef>

enum State {
  Begin,
  RTI,
  Parentheses,
  EndOfParentheses,
  Operation,
  Literal,
  String_literal,
  EndOfStringLiteral
};

bool letter(char a) {
    return ('a' <= a && a <= 'z') || ('A' <= a && a <= 'Z');
}

bool number(char a) {
    return '0' <= a && a <= '9';
}

void FST(const char* str, size_t now = 0, State s = Begin) {
    switch (s) {
        case Begin:
            if (letter(str[now])) {
                FST(str, now + 1, RTI);
            } else if (number(str[now])) {
                FST(str, now + 1, Literal);
            } else if (str[now] == ';' || str[now] == '}') {
                FST(str, now + 1, Begin);
            }
            break;
        case RTI:
            break;
        case Parentheses:
            break;
        case EndOfParentheses:
            break;
        case Operation:
            break;
        case Literal:
            break;
        case String_literal:
            break;
        case EndOfStringLiteral:
            break;
    }
}
