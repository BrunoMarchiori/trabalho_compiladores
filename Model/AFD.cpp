#include "AFD.hpp"

using namespace std;

bool AFD::accepts(const string& input) const {
    // No AFD, começamos em um único estado específico
    int currentState = startState;
    
    // Consome a string símbolo por símbolo
    for (char symbol : input) {
        currentState = transitionOn(currentState, symbol);
        
        // Se a transição retornar -1, significa que não há caminho definido para 
        // esse símbolo a partir do estado atual (dead state implícito). A palavra é rejeitada.
        if (currentState == -1) {
            return false;
        }
    }
    // Ao final da palavra, verifica se o estado em que paramos é de aceitação
    return acceptStates.find(currentState) != acceptStates.end();
}

int AFD::transitionOn(int from, char symbol) const {
    // Segurança: verifica se o estado 'from' existe no vetor
    if (from >= 0 && from < states.size()) {
        const State& currentState = states[from];
        
        // Busca a transição correspondente ao símbolo
        for (const auto& transition : currentState.transitions) {
            if (transition.first == symbol) {
                // Como é um AFD, garantimos que há apenas uma transição por símbolo.
                // Podemos retornar imediatamente o estado de destino.
                return transition.second;
            }
        }
    }
    // Retorna -1 indicando ausência de transição
    return -1;
}

set<int> AFD::getMapping(int afdState) const {
    // Busca o mapeamento gerado pelo Subset Construction (Thompson -> AFD)
    auto it = stateMapping.find(afdState);
    
    if (it != stateMapping.end()) {
        return it->second;
    }
    // Retorna conjunto vazio se o estado não existir no mapa
    return {};
}

bool AFD::isComplete() const {
    if (states.empty()) return false;

    // 1. Descobrir o alfabeto implícito do autômato
    set<char> alphabet;
    for (const State& state : states) {
        for (const auto& transition : state.transitions) {
            alphabet.insert(transition.first);
        }
    }

    // 2. Verificar se TODOS os estados possuem uma transição para CADA símbolo do alfabeto
    for (const State& state : states) {
        set<char> stateSymbols;
        
        // Coleta todos os símbolos que saem deste estado
        for (const auto& transition : state.transitions) {
            stateSymbols.insert(transition.first);
        }
        // Se a quantidade de símbolos saindo deste estado for menor que o alfabeto total,
        // significa que falta pelo menos uma transição, logo, não é completo.
        if (stateSymbols.size() != alphabet.size()) {
            return false;
        }
    }
    return true;
}