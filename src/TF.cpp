#include "TF.hpp"
#include "exceptions.hpp"

TF::TF() {
    current = new Node;
    current->parent = nullptr;
}

TF::~TF() {
    current = new Node;
    current->parent = nullptr;
}

void TF::next_scope() {
    const auto nw = new Node;
    nw->parent = current;
    current = nw;
}

void TF::exit_scope() {
    const auto nw = current;
    current = current->parent;
    delete nw;
}

std::set<TF::Function>::iterator TF::exists(const Function& var, Node* scope) const {
    if (scope == nullptr) {
        scope = current;
    }
    auto curr_position = scope->functions.begin();

    while (curr_position != scope->functions.end()) {
        if (*curr_position == var)
        {
            break;
        }
        ++curr_position;
    }

    return curr_position;
}

std::wstring TF::used(const std::wstring& name, const std::vector<std::wstring>& wstring_arguments, size_t line) const {
    std::vector <FunctionArgument> arguments;
    for (const std::wstring& wstring_arg : wstring_arguments) {
        arguments.push_back({wstring_arg});
    }
    Function func{name, 0, L"", arguments};

    auto nw = current;
    while (nw != nullptr && exists(func, nw) == nw->functions.end()) nw = nw->parent;
    if (nw == nullptr) {
        std::vector <std::wstring> args_types;
        for (const FunctionArgument& arg : func.arguments) {
            args_types.push_back(arg.type);
        }
        throw undeclared_function(func.name, func.line, args_types);
    }
    return exists(func, nw)->type;
}

void TF::add(const Function& func) const {
    if (auto other = exists(func); other != current->functions.end()) {
        std::vector <std::wstring> args;
        for (const auto& arg : func.arguments) {
            args.push_back(arg.type);
        }
        throw redeclaration_function(func.name, args, other->line, func.line);
    }
    current->functions.insert(func);
}
