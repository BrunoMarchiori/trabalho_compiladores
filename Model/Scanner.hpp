#ifndef SCANNER_HPP
#define SCANNER_HPP

#include "Token.h"
#include "MinimizedAFD.hpp"
#include <vector>
#include <memory>
#include <string>
#include <map>

using namespace std;

/**
 * Scanner: Lexicalizador de código fonte
 * Combina múltiplos autômatos minimizados (AFD-Min)
 * para reconhecer diferentes tipos de tokens
 */
class Scanner {
private:
    // Mapeia tipo de token → autômato minimizado
    map<string, shared_ptr<MinimizedAFD>> rules;
    
public:
    Scanner() = default;
    
    explicit Scanner(shared_ptr<MinimizedAFD> singleRule);
    
    /**
     * Adiciona uma regra de reconhecimento de token
     */
    void addRule(const string& tokenType, shared_ptr<MinimizedAFD> automaton);
    
    /**
     * Lexicaliza um código fonte e retorna lista de tokens
     */
    vector<Token> scan(const string& sourceCode);
    
    /**
     * Retorna o número de regras
     */
    int getRuleCount() const;
};

#endif // SCANNER_HPP