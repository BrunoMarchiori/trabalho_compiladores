#ifndef AUTOMATON_BASE_HPP
#define AUTOMATON_BASE_HPP

#include "State.hpp"
#include <vector>
#include <set>
#include <string>

/**
 * Classe base para todos os tipos de autômato
 * Define a interface comum para consultas e operações
 */
class AutomatonBase {
protected:
    int startState;
    std::set<int> acceptStates;
    std::vector<State> states;
    std::string tokenType;
    
public:
    AutomatonBase() : startState(-1), tokenType("") {}
    virtual ~AutomatonBase() = default;
    
    /**
     * Retorna o estado inicial
     */
    int getStartState() const { return startState; }
    
    /**
     * Retorna os estados de aceitação
     */
    const std::set<int>& getAcceptStates() const { return acceptStates; }
    
    /**
     * Retorna todos os estados
     */
    const std::vector<State>& getStates() const { return states; }
    
    /**
     * Define o tipo de token aceito
     */
    void setTokenType(const std::string& type) { tokenType = type; }
    
    /**
     * Obtém o tipo de token
     */
    std::string getTokenType() const { return tokenType; }
    
    /**
     * Número total de estados
     */
    int getStateCount() const { return states.size(); }
};

#endif // AUTOMATON_BASE_HPP
