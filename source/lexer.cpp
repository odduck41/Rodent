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

const char* removeComments(const char* line) {
    std::string str(line);
    std::string result;
    bool inLineComment = false;

    for (size_t i = 0; i < str.length(); ++i) {
        if (inLineComment) {
            if (str[i] == '\n') {
                inLineComment = false;
                result += str[i];
            }
        } else {
            if (str[i] == '/' && i + 1 < str.length() && str[i + 1] == '/') {
                inLineComment = true;
                ++i;
            } else {
                result += str[i];
            }
        }
    }

    return result.c_str();
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

    std::cout << lines << '\n';
    std::cout << "========================" << '\n';

    const char *lines_new = removeComments(lines);

    std::cout << lines_new << '\n';
    std::cout << "========================" << '\n';

    lexer::FSM state_machine(lines_new, sz);

    auto lexemes = state_machine.getLexems();

    std::string ret;

    for (auto& token : lexemes) {
        ret += "<Type: ";
        if (token.type == reserved) {
            ret += "reserved";
        } else if (token.type == identifier) {
            ret += "identifier";
        } else if (token.type == literal) {
            ret += "literal";
        } else if (token.type == type) {
            ret += "type";
        } else if (token.type == s_literal) {
            ret += "SLiteral";
        } else if (token.type == operation) {
            ret += "operation";
        } else if (token.type == punctuation) {
            ret += "punctuation";
        } else if (token.type == dot) {
            ret += "dot";
        } else if (token.type == comma) {
            ret += "comma";
        } else {
            ret += "parentheses";
        }

        ret += ", Content: \"" + std::string(token.content) + "\"" +
                ", Line: " + std::to_string(token.line) + ">\n";
    }

    delete[] lines;
    delete[] lines_new;

    return ret;
}

