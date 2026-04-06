#include "Scanner.hpp"
#include <algorithm>

using namespace std;

Scanner::Scanner(shared_ptr<MinimizedAFD> singleRule) {
    if (singleRule) {
        rules["DEFAULT"] = singleRule;
    }
}

void Scanner::addRule(
    const string& tokenType, 
    shared_ptr<MinimizedAFD> automaton
) {
    if (automaton) {
        rules[tokenType] = automaton;
    }
}

vector<Token> Scanner::scan(const string& sourceCode) {
    // TODO: Implementar lexicalização
    return {};
}

int Scanner::getRuleCount() const {
    return rules.size();
}