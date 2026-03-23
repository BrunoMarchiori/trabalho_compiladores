#ifndef SCANNER_HPP
#define SCANNER_HPP

#include "Token.h"
#include "MinimizedAFD.hpp"
#include <vector>
#include <memory>
#include <string>
#include <map>

/**
 * Scanner: Lexicalizador de código fonte
 * Combina múltiplos autômatos minimizados (AFD-Min)
 * para reconhecer diferentes tipos de tokens
 */
class Scanner {
private:
    // Mapeia tipo de token → autômato minimizado
    std::map<std::string, std::shared_ptr<MinimizedAFD>> rules;
    
public:
    Scanner() = default;
    
    explicit Scanner(std::shared_ptr<MinimizedAFD> singleRule);
    
    /**
     * Adiciona uma regra de reconhecimento de token
     */
    void addRule(const std::string& tokenType, std::shared_ptr<MinimizedAFD> automaton);
    
    /**
     * Lexicaliza um código fonte e retorna lista de tokens
     */
    std::vector<Token> scan(const std::string& sourceCode);
    
    /**
     * Retorna o número de regras
     */
    int getRuleCount() const;
};

#endif // SCANNER_HPP