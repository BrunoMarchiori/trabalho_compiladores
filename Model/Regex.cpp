#include "Regex.hpp"

Regex::Regex(const std::string& expression) 
    : originalExpression(expression), tokenType("") {}

void Regex::setExpression(const std::string& expression) {
    originalExpression = expression;
    rpn.clear();
}

std::string Regex::getExpression() const {
    return originalExpression;
}

void Regex::toRPN() {
    // TODO: Implementar conversão para RPN (Shunting-yard)
    rpn.clear();
}

const std::vector<std::string>& Regex::getRPN() const {
    return rpn;
}

void Regex::setTokenType(const std::string& type) {
    tokenType = type;
}

std::string Regex::getTokenType() const {
    return tokenType;
}