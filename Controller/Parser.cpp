#include "Parser.hpp"
#include <stdexcept>
#include <algorithm>

Parser::Parser(const std::vector<Token>& tokenList) 
    : tokens(tokenList), currentToken(0), lastError("") {}

Token& Parser::peek() {
    if (currentToken >= tokens.size()) {
        throw std::runtime_error("EOF reached unexpectedly");
    }
    return tokens[currentToken];
}

Token Parser::consume() {
    Token t = peek();
    currentToken++;
    return t;
}

bool Parser::match(const std::string& expectedType) {
    if (check(expectedType)) {
        consume();
        return true;
    }
    return false;
}

bool Parser::check(const std::string& tokenType) {
    if (currentToken >= tokens.size()) return false;
    return tokens[currentToken].typeName == tokenType;
}

void Parser::error(const std::string& message) {
    lastError = message + " at token: " + peek().lexeme;
}

void Parser::synchronize() {
    // TODO: Implementar sincronização de erros
}

std::shared_ptr<SyntaxTree> Parser::program() {
    // TODO: Implementar
    return nullptr;
}

std::shared_ptr<SyntaxTree> Parser::expression() {
    // TODO: Implementar
    return nullptr;
}

std::shared_ptr<SyntaxTree> Parser::parseDefine() {
    // TODO: Implementar
    return nullptr;
}

std::shared_ptr<SyntaxTree> Parser::parseLambda() {
    // TODO: Implementar
    return nullptr;
}

std::shared_ptr<SyntaxTree> Parser::parseIf() {
    // TODO: Implementar
    return nullptr;
}

std::shared_ptr<SyntaxTree> Parser::parseCond() {
    // TODO: Implementar
    return nullptr;
}

std::shared_ptr<SyntaxTree> Parser::parseLet() {
    // TODO: Implementar
    return nullptr;
}

std::shared_ptr<SyntaxTree> Parser::parseFunctionCall() {
    // TODO: Implementar
    return nullptr;
}

std::shared_ptr<SyntaxTree> Parser::parseLiteral() {
    // TODO: Implementar
    return nullptr;
}

std::shared_ptr<SyntaxTree> Parser::parseSymbol() {
    // TODO: Implementar
    return nullptr;
}

std::shared_ptr<SyntaxTree> Parser::parse() {
    try {
        return program();
    } catch (const std::exception& e) {
        error(std::string(e.what()));
        return nullptr;
    }
}

std::string Parser::getLastError() const {
    return lastError;
}

bool Parser::hasError() const {
    return !lastError.empty();
}
