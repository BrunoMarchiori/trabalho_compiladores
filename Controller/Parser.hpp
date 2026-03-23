#ifndef PARSER_HPP
#define PARSER_HPP

#include "../Model/Token.h"
#include "SyntaxTree.hpp"
#include <vector>
#include <memory>
#include <string>

/**
 * Parser Top-Down para Racket
 * Implementa análise preditiva recursiva descendente
 */
class Parser {
private:
    std::vector<Token> tokens;
    size_t currentToken = 0;
    std::string lastError;
    
    /**
     * Obtém o token atual
     */
    Token& peek();
    
    /**
     * Consome e retorna o token atual
     */
    Token consume();
    
    /**
     * Avança para o próximo token se o atual corresponder ao esperado
     */
    bool match(const std::string& expectedType);
    
    /**
     * Verifica se o token atual é do tipo esperado
     */
    bool check(const std::string& tokenType);
    
    /**
     * Reporta um erro e armazena para recover
     */
    void error(const std::string& message);
    
    /**
     * Recupera de erros sincronizando pontos de sincronização
     */
    void synchronize();
    
    // ======== Métodos de Parsing (Gramática Racket) ========
    
    /// Regra inicial: um programa é uma sequência de expressões
    std::shared_ptr<SyntaxTree> program();
    
    /// Uma expressão pode ser um literal, símbolo, ou forma especial
    std::shared_ptr<SyntaxTree> expression();
    
    /// Forma especial: (define ...)
    std::shared_ptr<SyntaxTree> parseDefine();
    
    /// Forma especial: (lambda ...)
    std::shared_ptr<SyntaxTree> parseLambda();
    
    /// Forma especial: (if ...)
    std::shared_ptr<SyntaxTree> parseIf();
    
    /// Forma especial: (cond ...)
    std::shared_ptr<SyntaxTree> parseCond();
    
    /// Forma especial: (let ...)
    std::shared_ptr<SyntaxTree> parseLet();
    
    /// Combinação de função e argumentos: (func arg1 arg2 ...)
    std::shared_ptr<SyntaxTree> parseFunctionCall();
    
    /// Literal: número ou string
    std::shared_ptr<SyntaxTree> parseLiteral();
    
    /// Símbolo ou identificador
    std::shared_ptr<SyntaxTree> parseSymbol();
    
public:
    explicit Parser(const std::vector<Token>& tokenList);
    
    /**
     * Inicia a análise sintática
     * Retorna a árvore sintática em caso de sucesso, nullptr em caso de erro
     */
    std::shared_ptr<SyntaxTree> parse();
    
    /**
     * Obtém a mensagem de erro do último parse
     */
    std::string getLastError() const;
    
    /**
     * Verifica se houve erro durante parse
     */
    bool hasError() const;
};

#endif // PARSER_HPP
