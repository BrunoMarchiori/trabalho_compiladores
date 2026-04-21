#include "AFND.hpp"
#include <algorithm>

using namespace std;

set<int> AFND::transitionOn(const set<int>& current_states, char symbol) const {
    set<int> nextStates;
    
    for (int stateId : current_states) {
        // Acesso direto assumindo que o ID do estado bate com o índice do vetor.
        if (stateId >= 0 && stateId < states.size()) {
            const State& currentState = states[stateId];
            
            // Itera sobre todas as transições que saem deste estado
            for (const auto& transition : currentState.transitions) {
                // transition.first é o símbolo, transition.second é o ID do estado destino
                if (transition.first == symbol) {
                    nextStates.insert(transition.second);
                }
            }
        }
    }
    return nextStates;
}

bool AFND::containsAcceptState(const set<int>& current_states) const {
    for (int stateId : current_states) {
        // Verifica no set 'acceptStates' herdado de AutomatonBase
        if (acceptStates.find(stateId) != acceptStates.end()) {
            return true;
        }
    }
    return false;
}

bool AFND::accepts(const string& input) const {
    // 1. Inicializa o conjunto de estados atuais com o estado inicial
    set<int> currentStates = { startState };
    
    // 2. Consome a string de entrada
    for (char symbol : input) {
        currentStates = transitionOn(currentStates, symbol);
        
        // Otimização: se o conjunto ficar vazio (dead state), rejeita a palavra
        if (currentStates.empty()) {
            return false;
        }
    }
    
    // 3. Verifica se algum dos estados onde paramos é de aceitação
    return containsAcceptState(currentStates);
}

set<char> AFND::getAlphabet() const {
    set<char> alphabet;
    
    // Passa por todos os estados do autômato
    for (const State& state : states) {
        // Passa por todas as transições do estado
        for (const auto& transition : state.transitions) {
            char symbol = transition.first;
            
            // Adiciona ao alfabeto, ignorando o epsilon ('\0') se ele tiver sobrado
            if (symbol != '\0') {
                alphabet.insert(symbol);
            }
        }
    }
    
    return alphabet;
}