#include "Regex.hpp"
#include <stack>
#include <cctype>
#include <algorithm>

using namespace std;

Regex::Regex(const string& expression) 
    : originalExpression(expression), tokenType("") {}

void Regex::setExpression(const string& expression) {
    originalExpression = expression;
    rpn.clear();
}

string Regex::getExpression() const {
    return originalExpression;
}

void Regex::toRPN() {
    rpn.clear();
    stack<string> operatorStack;
    
    // Precedência dos operadores: * > . (concatenação) > |
    auto getPrecedence = [](const string& op) -> int {
        if (op == "|") return 1;
        if (op == ".") return 2;
        if (op == "*" || op == "+") return 3;
        return 0;
    };
    
    // Associatividade: direita para * e +, esquerda para | e .
    auto isRightAssoc = [](const string& op) -> bool {
        return op == "*" || op == "+";
    };
    
    for (size_t i = 0; i < originalExpression.length(); i++) {
        char c = originalExpression[i];
        
        // Se é um caracter normal (não operador/parêntese)
        if (isalnum(c) || c == '_' || c == '-') {
            rpn.push_back(string(1, c));
        }
        // Parêntese aberto
        else if (c == '(') {
            operatorStack.push("(");
        }
        // Parêntese fechado
        else if (c == ')') {
            while (!operatorStack.empty() && operatorStack.top() != "(") {
                rpn.push_back(operatorStack.top());
                operatorStack.pop();
            }
            if (!operatorStack.empty()) {
                operatorStack.pop(); // Remove o '('
            }
        }
        // Operadores: |, *, +
        else if (c == '|' || c == '*' || c == '+') {
            string op(1, c);
            
            while (!operatorStack.empty() && 
                   operatorStack.top() != "(" &&
                   getPrecedence(operatorStack.top()) > getPrecedence(op)) {
                rpn.push_back(operatorStack.top());
                operatorStack.pop();
            }
            operatorStack.push(op);
        }
        // Escape: \. \* \| etc
        else if (c == '\\' && i + 1 < originalExpression.length()) {
            char nextChar = originalExpression[++i];
            rpn.push_back(string(1, nextChar));
        }
        // Caracteres especiais como [0-9], [a-z]
        else if (c == '[') {
            // Encontra o fechamento ]
            size_t closePos = originalExpression.find(']', i);
            if (closePos != string::npos) {
                rpn.push_back(originalExpression.substr(i, closePos - i + 1));
                i = closePos;
            }
        }
    }
    
    // Desempilha operadores restantes
    while (!operatorStack.empty()) {
        rpn.push_back(operatorStack.top());
        operatorStack.pop();
    }
}

const vector<string>& Regex::getRPN() const {
    return rpn;
}

void Regex::setTokenType(const string& type) {
    tokenType = type;
}

string Regex::getTokenType() const {
    return tokenType;
}