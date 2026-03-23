#include "Scanner.hpp"
#include <algorithm>

Scanner::Scanner(std::shared_ptr<MinimizedAFD> singleRule) {
    if (singleRule) {
        rules["DEFAULT"] = singleRule;
    }
}

void Scanner::addRule(
    const std::string& tokenType, 
    std::shared_ptr<MinimizedAFD> automaton
) {
    if (automaton) {
        rules[tokenType] = automaton;
    }
}

std::vector<Token> Scanner::scan(const std::string& sourceCode) {
    // TODO: Implementar lexicalização
    return {};
}

int Scanner::getRuleCount() const {
    return rules.size();
}