#ifndef STATE_HPP
#define STATE_HPP

#include <vector>
#include <utility>
#include <string>

/**
 * Representa um estado em um autômato (NFA, DFA, etc)
 * Suporta transições com símbolos e transições epsilon
 */
struct State {
    int id;
    bool isAccepting;
    std::string tokenType;  // Tipo de token que este estado aceita
    
    // Transições: (símbolo, estado_destino)
    // Para epsilon, usamos '\0' como símbolo
    std::vector<std::pair<char, int>> transitions;
    
    explicit State(int id) 
        : id(id), isAccepting(false), tokenType("") {}
    
    // Adiciona uma transição regular
    void addTransition(char symbol, int targetState) {
        transitions.push_back({symbol, targetState});
    }
    
    // Adiciona uma transição epsilon
    void addEpsilonTransition(int targetState) {
        addTransition('\0', targetState);  // '\0' representa epsilon
    }
};

#endif // STATE_HPP
