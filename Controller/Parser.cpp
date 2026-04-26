#include "Parser.hpp"
#include <sstream>

using namespace std;

Parser::Parser(const vector<Token>& tokenList) 
    : tokens(tokenList), currentToken(0), lastError("") {}

Token& Parser::peek() {
    static Token eofFallback {TokenType::TOKEN_EOF, "EOF", "", 0, 0};
    if (tokens.empty()) {
        return eofFallback;
    }
    if (currentToken >= tokens.size()) {
        return tokens.back();
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

bool Parser::expect(const string& expectedType, const string& message) {
    if (check(expectedType)) {
        consume();
        return true;
    }
    error(message + " (esperado: " + expectedType + ")");
    return false;
}

bool Parser::isExpressionStart() const {
    if (currentToken >= tokens.size()) return false;

    const string& type = tokens[currentToken].typeName;
    if (type == "LPAREN") return true;
    if (type == "INTEGER" || type == "FLOAT" || type == "STRING") return true;
    if (type == "BOOLEAN_TRUE" || type == "BOOLEAN_FALSE") return true;
    if (type == "SYMBOL" || type == "IDENTIFIER") return true;
    if (type.rfind("OPERATOR_", 0) == 0) return true;
    return false;
}

void Parser::error(const string& message) {
    ostringstream oss;
    if (currentToken < tokens.size()) {
        const Token& tok = tokens[currentToken];
        oss << "Linha " << tok.line << ", Coluna " << tok.column << ": "
            << message << " (token: " << tok.typeName << " \"" << tok.lexeme << "\")";
    } else {
        oss << "Fim de arquivo: " << message;
    }
    string fullMessage = oss.str();
    errors.push_back(fullMessage);
    if (lastError.empty()) {
        lastError = fullMessage;
    }
}

void Parser::synchronize() {
    while (!check("EOF")) {
        if (check("RPAREN")) {
            consume();
            return;
        }
        if (isExpressionStart()) {
            return;
        }
        consume();
    }
}

shared_ptr<SyntaxTree> Parser::program() {
    auto root = make_shared<SyntaxTree>(SyntaxTree::NodeType::PROGRAM);

    while (!check("EOF")) {
        auto expr = expression();
        if (expr) {
            root->addChild(expr);
            continue;
        }

        if (!check("EOF")) {
            synchronize();
        }
    }

    return root;
}

shared_ptr<SyntaxTree> Parser::expression() {
    if (check("EOF")) {
        error("Expressão esperada, mas encontrou EOF");
        return nullptr;
    }

    if (check("LPAREN")) {
        consume(); // Consome '('
        if (check("KEYWORD_DEFINE")) return parseDefine();
        if (check("KEYWORD_LAMBDA")) return parseLambda();
        if (check("KEYWORD_IF")) return parseIf();
        if (check("KEYWORD_COND")) return parseCond();
        if (check("KEYWORD_LET")) return parseLet();
        return parseFunctionCall();
    }

    if (check("INTEGER") || check("FLOAT") || check("STRING") ||
        check("BOOLEAN_TRUE") || check("BOOLEAN_FALSE")) {
        return parseLiteral();
    }

    if (check("SYMBOL") || check("IDENTIFIER") || check("KEYWORD_ELSE") ||
        check("KEYWORD_AND") || check("KEYWORD_OR") || check("KEYWORD_NOT") ||
        check("KEYWORD_CASE") || check("KEYWORD_BEGIN") || check("KEYWORD_QUOTE") ||
        check("KEYWORD_QUASIQUOTE") || check("KEYWORD_UNQUOTE") || check("KEYWORD_LETSTAR") ||
        check("KEYWORD_LETREC") || (currentToken < tokens.size() && tokens[currentToken].typeName.rfind("OPERATOR_", 0) == 0)) {
        return parseSymbol();
    }

    error("Início de expressão inválido");
    return nullptr;
}

shared_ptr<SyntaxTree> Parser::parseDefine() {
    auto node = make_shared<SyntaxTree>(SyntaxTree::NodeType::DEFINE);
    expect("KEYWORD_DEFINE", "Forma define inválida");

    // (define (f x y) body...)
    if (check("LPAREN")) {
        consume();

        if (!check("SYMBOL")) {
            error("Nome de função esperado em define");
        } else {
            node->addChild(parseSymbol()); // nome da função
        }

        auto params = make_shared<SyntaxTree>(SyntaxTree::NodeType::LIST, "params");
        while (!check("RPAREN") && !check("EOF")) {
            if (!check("SYMBOL")) {
                error("Parâmetro inválido em define");
                consume();
                continue;
            }
            params->addChild(parseSymbol());
        }
        node->addChild(params);
        expect("RPAREN", "Faltando ')' após lista de parâmetros de define");

        bool hasBody = false;
        while (!check("RPAREN") && !check("EOF")) {
            auto bodyExpr = expression();
            if (bodyExpr) {
                node->addChild(bodyExpr);
                hasBody = true;
            } else {
                synchronize();
            }
        }
        if (!hasBody) {
            error("define de função precisa de corpo");
        }
        expect("RPAREN", "Faltando ')' para fechar define");
        return node;
    }

    // (define var expr)
    if (!check("SYMBOL")) {
        error("Identificador esperado após define");
    } else {
        node->addChild(parseSymbol());
    }

    auto valueExpr = expression();
    if (!valueExpr) {
        error("Expressão esperada no valor de define");
    } else {
        node->addChild(valueExpr);
    }

    expect("RPAREN", "Faltando ')' para fechar define");
    return node;
}

shared_ptr<SyntaxTree> Parser::parseLambda() {
    auto node = make_shared<SyntaxTree>(SyntaxTree::NodeType::LAMBDA);
    expect("KEYWORD_LAMBDA", "Forma lambda inválida");
    expect("LPAREN", "Faltando '(' para lista de parâmetros de lambda");

    auto params = make_shared<SyntaxTree>(SyntaxTree::NodeType::LIST, "params");
    while (!check("RPAREN") && !check("EOF")) {
        if (!check("SYMBOL")) {
            error("Parâmetro inválido em lambda");
            consume();
            continue;
        }
        params->addChild(parseSymbol());
    }
    node->addChild(params);
    expect("RPAREN", "Faltando ')' após parâmetros de lambda");

    bool hasBody = false;
    while (!check("RPAREN") && !check("EOF")) {
        auto bodyExpr = expression();
        if (bodyExpr) {
            node->addChild(bodyExpr);
            hasBody = true;
        } else {
            synchronize();
        }
    }

    if (!hasBody) {
        error("lambda precisa de pelo menos uma expressão no corpo");
    }
    expect("RPAREN", "Faltando ')' para fechar lambda");
    return node;
}

shared_ptr<SyntaxTree> Parser::parseIf() {
    auto node = make_shared<SyntaxTree>(SyntaxTree::NodeType::IF);
    expect("KEYWORD_IF", "Forma if inválida");

    auto condExpr = expression();
    auto thenExpr = expression();
    auto elseExpr = expression();

    if (!condExpr || !thenExpr || !elseExpr) {
        error("if exige exatamente 3 expressões: condição, então, senão");
    } else {
        node->addChild(condExpr);
        node->addChild(thenExpr);
        node->addChild(elseExpr);
    }

    expect("RPAREN", "Faltando ')' para fechar if");
    return node;
}

shared_ptr<SyntaxTree> Parser::parseCond() {
    auto node = make_shared<SyntaxTree>(SyntaxTree::NodeType::COND);
    expect("KEYWORD_COND", "Forma cond inválida");

    bool hasClause = false;
    while (!check("RPAREN") && !check("EOF")) {
        bool bracketClause = false;
        if (check("LBRACKET")) {
            bracketClause = true;
            consume();
        } else if (check("LPAREN")) {
            consume();
        } else {
            error("Cláusula de cond deve iniciar com '[' ou '('");
            synchronize();
            continue;
        }

        auto clause = make_shared<SyntaxTree>(SyntaxTree::NodeType::COND_CLAUSE);
        auto testExpr = expression();
        if (!testExpr) {
            error("Cláusula cond sem teste");
        } else {
            clause->addChild(testExpr);
        }

        bool hasClauseBody = false;
        const string closeTok = bracketClause ? "RBRACKET" : "RPAREN";
        while (!check(closeTok) && !check("EOF")) {
            auto clauseExpr = expression();
            if (clauseExpr) {
                clause->addChild(clauseExpr);
                hasClauseBody = true;
            } else {
                synchronize();
            }
        }

        if (!hasClauseBody) {
            error("Cláusula cond deve ter ao menos uma expressão");
        }
        expect(closeTok, "Faltando fechamento da cláusula cond");
        node->addChild(clause);
        hasClause = true;
    }

    if (!hasClause) {
        error("cond deve ter ao menos uma cláusula");
    }
    expect("RPAREN", "Faltando ')' para fechar cond");
    return node;
}

shared_ptr<SyntaxTree> Parser::parseLet() {
    auto node = make_shared<SyntaxTree>(SyntaxTree::NodeType::LET);
    expect("KEYWORD_LET", "Forma let inválida");
    expect("LPAREN", "Faltando '(' para lista de bindings de let");

    auto bindings = make_shared<SyntaxTree>(SyntaxTree::NodeType::BINDING_LIST);
    while (!check("RPAREN") && !check("EOF")) {
        bool bracketBinding = false;
        if (check("LBRACKET")) {
            bracketBinding = true;
            consume();
        } else if (check("LPAREN")) {
            consume();
        } else {
            error("Binding de let deve iniciar com '[' ou '('");
            synchronize();
            continue;
        }

        auto binding = make_shared<SyntaxTree>(SyntaxTree::NodeType::LIST, "binding");
        if (!check("SYMBOL")) {
            error("Identificador esperado em binding de let");
        } else {
            binding->addChild(parseSymbol());
        }

        auto valueExpr = expression();
        if (!valueExpr) {
            error("Expressão esperada em binding de let");
        } else {
            binding->addChild(valueExpr);
        }

        const string closeTok = bracketBinding ? "RBRACKET" : "RPAREN";
        expect(closeTok, "Faltando fechamento do binding de let");
        bindings->addChild(binding);
    }

    expect("RPAREN", "Faltando ')' após lista de bindings de let");
    node->addChild(bindings);

    bool hasBody = false;
    while (!check("RPAREN") && !check("EOF")) {
        auto bodyExpr = expression();
        if (bodyExpr) {
            node->addChild(bodyExpr);
            hasBody = true;
        } else {
            synchronize();
        }
    }

    if (!hasBody) {
        error("let precisa de pelo menos uma expressão no corpo");
    }
    expect("RPAREN", "Faltando ')' para fechar let");
    return node;
}

shared_ptr<SyntaxTree> Parser::parseFunctionCall() {
    auto node = make_shared<SyntaxTree>(SyntaxTree::NodeType::FUNCTION_CALL);

    if (check("RPAREN")) {
        error("Aplicação de função vazia não é permitida");
        consume();
        return node;
    }

    auto callee = expression();
    if (!callee) {
        error("Função/chamada inválida");
    } else {
        node->addChild(callee);
    }

    while (!check("RPAREN") && !check("EOF")) {
        auto arg = expression();
        if (arg) {
            node->addChild(arg);
        } else {
            synchronize();
        }
    }
    expect("RPAREN", "Faltando ')' para fechar chamada de função");
    return node;
}

shared_ptr<SyntaxTree> Parser::parseLiteral() {
    Token tok = consume();
    if (tok.typeName == "INTEGER" || tok.typeName == "FLOAT") {
        return make_shared<SyntaxTree>(SyntaxTree::NodeType::LITERAL_NUM, tok.lexeme);
    }
    if (tok.typeName == "STRING") {
        return make_shared<SyntaxTree>(SyntaxTree::NodeType::LITERAL_STRING, tok.lexeme);
    }
    if (tok.typeName == "BOOLEAN_TRUE" || tok.typeName == "BOOLEAN_FALSE") {
        return make_shared<SyntaxTree>(SyntaxTree::NodeType::LITERAL_BOOL, tok.lexeme);
    }

    error("Literal inválido");
    return nullptr;
}

shared_ptr<SyntaxTree> Parser::parseSymbol() {
    if (check("EOF")) {
        error("Símbolo esperado, mas encontrou EOF");
        return nullptr;
    }

    Token tok = consume();
    return make_shared<SyntaxTree>(SyntaxTree::NodeType::SYMBOL, tok.lexeme);
}

shared_ptr<SyntaxTree> Parser::parse() {
    errors.clear();
    lastError.clear();
    currentToken = 0;
    return program();
}

string Parser::getLastError() const {
    return lastError;
}

vector<string> Parser::getErrors() const {
    return errors;
}

bool Parser::hasError() const {
    return !errors.empty();
}
