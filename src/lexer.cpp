#include <cstddef>
#include <string>

#include "lexer.hpp"
#include "files.hpp"

Trie* reserved = new Trie;
Trie* types = new Trie;

void removeComments(const size_t sz, wchar_t*& program) {
    std::wstring result;
    result.reserve(sz + 1);

    bool single{}, multiline{};
    for (size_t i = 0; program[i] != '\0'; ++i) {
        if (program[i] == '\r') {
            continue;
        }

        if (single) {
            if (program[i] == '\n') {
                single = false;
            } else {
                continue;
            }
        }

        if (multiline) {
            if (program[i] == '\n') {
                result += program[i];
                continue;
            }

            if (program[i] == '*' && program[i + 1] == '/') {
                multiline = false;
                ++i;
            }

            continue;
        }

        if (program[i] == '/' && program[i + 1] == '/') {
            single = true;
            continue;
        }

        if (program[i] == '/' && program[i + 1] == '*') {
            multiline = true;
            continue;
        }

        result += program[i];
    }


    result.copy(program, result.size());
    program[result.size()] = '\0';
}

void process(RFile& file, const long long size_, Trie* trie) {
    const auto str = new wchar_t[size_ + 1];
    file.read(str, size_);
    str[size_] = '\0';

    addAll(str, trie);

    delete[] str;
}

void loadReserved(const char* filename) {
    RFile file(filename);

    process(file, file.size(), reserved);
}

void loadTypes(const char* filename) {
    RFile file(filename);

    process(file, file.size(), types);
}

void addAll(const wchar_t* str, Trie* trie) {
    Trie* current = trie;
    for (size_t i = 0; str[i] != '\0'; ++i) {
        if (str[i] == '\n' || str[i] == '\r' || str[i] == ' ') {
            current->terminal = true;
            current = trie;
        } else {
            add(str[i], current);
        }
    }
    current->terminal = true;
}

void add(const wchar_t s, Trie*& level) {
    if (level->children[s - 'a'] == nullptr) {
        level->children[s - 'a'] = new Trie;
    }
    level = level->children[s - 'a'];
}

bool inTrie(const wchar_t* s, const Trie* trie, const size_t sz) {
    if (s[sz] == '\0' && trie->terminal) {
        return true;
    }
    if (s[sz] == '\0') {
        return false;
    }

    if (trie->children[s[sz] - 'a'] == nullptr) return false;
    return inTrie(s, trie->children[s[sz] - 'a'], sz + 1);
}

bool inTrie(const std::wstring& s, const Trie* trie, size_t sz) {
    if (sz == s.size() && trie->terminal) {
        return true;
    }
    if (sz == s.size()) {
        return false;
    }

    if (trie->children[s[sz] - 'a'] == nullptr) return false;
    return inTrie(s, trie->children[s[sz] - 'a'], sz + 1);
}
