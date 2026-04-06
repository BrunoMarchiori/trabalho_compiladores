#ifndef AUTOMATON_BASE_HPP
#define AUTOMATON_BASE_HPP

#include "State.hpp"
#include <vector>
#include <set>
#include <string>

using namespace std;

/**
 * Classe base para todos os tipos de autômato
 * Define a interface comum para consultas e operações
 */
class AutomatonBase {
protected:
    int startState;
    set<int> acceptStates;
    vector<State> states;
    string tokenType;
    
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
    const set<int>& getAcceptStates() const { return acceptStates; }
    
    /**
     * Retorna todos os estados
     */
    const vector<State>& getStates() const { return states; }
    
    /**
     * Define o tipo de token aceito
     */
    void setTokenType(const string& type) { tokenType = type; }
    
    /**
     * Obtém o tipo de token
     */
    string getTokenType() const { return tokenType; }
    
    /**
     * Número total de estados
     */
    int getStateCount() const { return states.size(); }
};

#endif // AUTOMATON_BASE_HPP
