#include <lexer.hpp>
#include <finiteStateMachine.hpp>
#include <fstream>

Trie* reserved_ = new Trie;
Trie* types_ = new Trie;

void load_reserved() {
    std::ifstream is("../reserved.txt", std::ifstream::ate | std::ifstream::binary);
    const long long sz = is.tellg();

    char* lines = new char[sz + 1];

    is.seekg(0, std::ios::beg);
    is.read(lines, sz);

    lines[sz] = '\0';

    addAll(lines, reserved_);

    delete[] lines;
}

void load_basic_types() {
    std::ifstream is("../types.txt", std::ifstream::ate | std::ifstream::binary);
    const long long sz = is.tellg();

    char* lines = new char[sz + 1];

    is.seekg(0, std::ios::beg);
    is.read(lines, sz);

    lines[sz] = '\0';

    addAll(lines, types_);

    delete[] lines;
}

void addAll(const char* lines, Trie* cur) {
    Trie* current = cur;
    for (size_t i = 0; lines[i] != '\0'; ++i) {
        if (lines[i] != ' ' && lines[i] != '\n' && lines[i] != '\r') {
            add(current, lines[i]);
        } else {
            current = cur;
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

bool inTrie(const char* word, const Trie* level, size_t current) {
    if (level->terminal && word[current] != '\0') {
        return false;
    }

    if (word[current] == '\0' && level->terminal) {
        return true;
    } else if (word[current] == '\0') {
        return false;
    }

    return level->children[word[current] - 'a'] != nullptr
    && inTrie(word, level->children[word[current] - 'a'], current + 1);
}

std::string tokenize(const char* from) {
    std::ifstream is(from, std::ifstream::ate | std::ifstream::binary);
    const long long sz = is.tellg();

    char* lines = new char[sz + 1];

    is.seekg(0, std::ios::beg);
    is.read(lines, sz);

    lines[sz] = '\0';

    lexer::FSM state_machine(lines, sz);

    auto lexemes = state_machine.getLexems();

    std::string ret;

    for (auto& token : lexemes) {
        ret += "<Type: " + std::to_string(token.type) +
                ", Content: " + std::string(token.content) +
                ", Line: " + std::to_string(token.line) + ">\n";
    }

    delete[] lines;

    return ret;
}

