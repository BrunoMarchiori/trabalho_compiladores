#ifndef PARSER_HPP
#define PARSER_HPP

#include "../Model/Token.h"
#include "SyntaxTree.hpp"
#include <vector>
#include <memory>
#include <string>

using namespace std;

/**
 * Parser Top-Down para Racket
 * Implementa análise preditiva recursiva descendente
 */
class Parser {
private:
    vector<Token> tokens;
    size_t currentToken = 0;
    string lastError;
    vector<string> errors;
    
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
    bool match(const string& expectedType);
    
    /**
     * Verifica se o token atual é do tipo esperado
     */
    bool check(const string& tokenType);

    /**
     * Consome token esperado; registra erro se não encontrar.
     */
    bool expect(const string& expectedType, const string& message);

    /**
     * Verifica se o token atual pode iniciar uma expressão.
     */
    bool isExpressionStart() const;
    
    /**
     * Reporta um erro e armazena para recover
     */
    void error(const string& message);
    
    /**
     * Recupera de erros sincronizando pontos de sincronização
     */
    void synchronize();
    
    // ======== Métodos de Parsing (Gramática Racket) ========
    
    /// Regra inicial: um programa é uma sequência de expressões
    shared_ptr<SyntaxTree> program();
    
    /// Uma expressão pode ser um literal, símbolo, ou forma especial
    shared_ptr<SyntaxTree> expression();
    
    /// Forma especial: (define ...)
    shared_ptr<SyntaxTree> parseDefine();
    
    /// Forma especial: (lambda ...)
    shared_ptr<SyntaxTree> parseLambda();
    
    /// Forma especial: (if ...)
    shared_ptr<SyntaxTree> parseIf();
    
    /// Forma especial: (cond ...)
    shared_ptr<SyntaxTree> parseCond();
    
    /// Forma especial: (let ...)
    shared_ptr<SyntaxTree> parseLet();
    
    /// Combinação de função e argumentos: (func arg1 arg2 ...)
    shared_ptr<SyntaxTree> parseFunctionCall();
    
    /// Literal: número ou string
    shared_ptr<SyntaxTree> parseLiteral();
    
    /// Símbolo ou identificador
    shared_ptr<SyntaxTree> parseSymbol();
    
public:
    explicit Parser(const vector<Token>& tokenList);
    
    /**
     * Inicia a análise sintática
     * Retorna a árvore sintática em caso de sucesso, nullptr em caso de erro
     */
    shared_ptr<SyntaxTree> parse();
    
    /**
     * Obtém a mensagem de erro do último parse
     */
    string getLastError() const;

    /**
     * Lista de erros coletados durante o parse
     */
    vector<string> getErrors() const;
    
    /**
     * Verifica se houve erro durante parse
     */
    bool hasError() const;
};

#endif // PARSER_HPP
