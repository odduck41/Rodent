#include <lexer.hpp>
#include <fstream>

Trie* reserved_ = new Trie;

void load() {
    std::ifstream is("../reserved.txt", std::ifstream::ate | std::ifstream::binary);
    const long long sz = is.tellg();

    char* lines = new char[sz + 1];

    is.seekg(0, std::ios::beg);
    is.read(lines, sz);

    lines[sz] = '\0';

    addAll(lines);

    delete[] lines;
}

void addAll(const char* lines) {
    Trie* current = reserved_;
    for (size_t i = 0; lines[i] != '\0'; ++i) {
        if (lines[i] != ' ' && lines[i] != '\n' && lines[i] != '\r') {
            add(current, lines[i]);
        } else {
            current = reserved_;
        }
    }
}

void add(Trie*& level, char cur) {
    if (level->children[cur - 'a'] == nullptr) {
        level->children[cur - 'a'] = new Trie;
        level->children[cur - 'a']->terminal = true;
        if (level->terminal) {
            level->terminal = false;
        }
    }
    level = level->children[cur - 'a'];
}

bool inTrie(const char* word, size_t current, const Trie* level) {
    if (level->terminal && word[current] != '\0') {
        return false;
    }

    if (word[current] == '\0' && level->terminal) {
        return true;
    } else if (word[current] == '\0') {
        return false;
    }

    return level->children[word[current] - 'a'] != nullptr
    && inTrie(word, current + 1, level->children[word[current] - 'a']);
}