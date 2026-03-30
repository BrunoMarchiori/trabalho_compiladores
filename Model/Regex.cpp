#include "Regex.hpp"

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
    // TODO: Implementar conversão para RPN (Shunting-yard)
    rpn.clear();
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