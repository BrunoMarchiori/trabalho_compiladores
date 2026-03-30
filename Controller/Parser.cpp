#include "Parser.hpp"
#include <stdexcept>
#include <algorithm>

using namespace std;

Parser::Parser(const vector<Token>& tokenList) 
    : tokens(tokenList), currentToken(0), lastError("") {}

Token& Parser::peek() {
    if (currentToken >= tokens.size()) {
        throw runtime_error("EOF reached unexpectedly");
    }
    return tokens[currentToken];
}

Token Parser::consume() {
    Token t = peek();
    currentToken++;
    return t;
}

bool Parser::match(const string& expectedType) {
    if (check(expectedType)) {
        consume();
        return true;
    }
    return false;
}

bool Parser::check(const string& tokenType) {
    if (currentToken >= tokens.size()) return false;
    return tokens[currentToken].typeName == tokenType;
}

void Parser::error(const string& message) {
    lastError = message + " at token: " + peek().lexeme;
}

void Parser::synchronize() {
    // TODO: Implementar sincronização de erros
}

shared_ptr<SyntaxTree> Parser::program() {
    // TODO: Implementar
    return nullptr;
}

shared_ptr<SyntaxTree> Parser::expression() {
    // TODO: Implementar
    return nullptr;
}

shared_ptr<SyntaxTree> Parser::parseDefine() {
    // TODO: Implementar
    return nullptr;
}

shared_ptr<SyntaxTree> Parser::parseLambda() {
    // TODO: Implementar
    return nullptr;
}

shared_ptr<SyntaxTree> Parser::parseIf() {
    // TODO: Implementar
    return nullptr;
}

shared_ptr<SyntaxTree> Parser::parseCond() {
    // TODO: Implementar
    return nullptr;
}

shared_ptr<SyntaxTree> Parser::parseLet() {
    // TODO: Implementar
    return nullptr;
}

shared_ptr<SyntaxTree> Parser::parseFunctionCall() {
    // TODO: Implementar
    return nullptr;
}

shared_ptr<SyntaxTree> Parser::parseLiteral() {
    // TODO: Implementar
    return nullptr;
}

shared_ptr<SyntaxTree> Parser::parseSymbol() {
    // TODO: Implementar
    return nullptr;
}

shared_ptr<SyntaxTree> Parser::parse() {
    try {
        return program();
    } catch (const exception& e) {
        error(string(e.what()));
        return nullptr;
    }
}

string Parser::getLastError() const {
    return lastError;
}

bool Parser::hasError() const {
    return !lastError.empty();
}
