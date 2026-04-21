#include "Regex.hpp"
#include <stack>
#include <cctype>
#include <iostream>

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
    
    // PASSO 1: Tokenizar a entrada (respeitar escapes e classes)
    vector<string> tokens;
    
    for (size_t i = 0; i < originalExpression.length(); i++) {
        char c = originalExpression[i];
        
        // Escape: \x
        if (c == '\\' && i + 1 < originalExpression.length()) {
            tokens.push_back(string("\\") + originalExpression[++i]);
        }
        // Classe de caracteres: [...]
        else if (c == '[') {
            string classStr = "[";
            i++;
            while (i < originalExpression.length() && originalExpression[i] != ']') {
                classStr += originalExpression[i];
                i++;
            }
            if (i < originalExpression.length()) {
                classStr += ']';
            }
            tokens.push_back(classStr);
        }
        // Operadores e parênteses
        else if (c == '|' || c == '(' || c == ')' || c == '*' || c == '+' || c == '?') {
            tokens.push_back(string(1, c));
        }
        // Ignorar espaços
        else if (!isspace(c)) {
            tokens.push_back(string(1, c));
        }
    }
    
    // PASSO 2: Adicionar operadores de concatenação explícitos (.)
    vector<string> expandedTokens;
    
    for (size_t i = 0; i < tokens.size(); i++) {
        const string& token = tokens[i];
        expandedTokens.push_back(token);
        
        // Verifica se precisa adicionar . depois deste token
        if (i + 1 < tokens.size()) {
            const string& nextToken = tokens[i + 1];
            
            // Adiciona ° se ambos podem ser concatenados
            // NÃO adiciona se:
            // - próximo é operador unário (*, +, ?)
            // - próximo é |
            // - próximo é )
            // - atual é (
            // - atual é |
            
            bool canAddConcat = !(token == "(" || token == "|");
            bool nextCanFollow = !(nextToken == ")" || nextToken == "|" || 
                                 nextToken == "*" || nextToken == "+" || nextToken == "?");
            
            if (canAddConcat && nextCanFollow) {
                expandedTokens.push_back("°");
            }
        }
    }
    
    // PASSO 3: Converter para RPN usando Shunting-yard
    stack<string> operatorStack;
    
    auto getPrecedence = [](const string& op) -> int {
        if (op == "|") return 1;
        if (op == "°") return 2; 
        if (op == "*" || op == "+" || op == "?") return 3;
        return 0;
    };
    
    for (const string& token : expandedTokens) {
        // Parêntese aberto
        if (token == "(") {
            operatorStack.push(token);
        }
        // Parêntese fechado
        else if (token == ")") {
            while (!operatorStack.empty() && operatorStack.top() != "(") {
                rpn.push_back(operatorStack.top());
                operatorStack.pop();
            }
            if (!operatorStack.empty()) {
                operatorStack.pop(); // Remove '('
            }
        }
        // Operadores
        else if (token == "|" || token == "°" || token == "*" || token == "+" || token == "?") {
            while (!operatorStack.empty() && 
                   operatorStack.top() != "(" &&
                   getPrecedence(operatorStack.top()) >= getPrecedence(token)) {
                rpn.push_back(operatorStack.top());
                operatorStack.pop();
            }
            operatorStack.push(token);
        }
        // Operando (caractere, classe, etc)
        else {
            rpn.push_back(token);
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