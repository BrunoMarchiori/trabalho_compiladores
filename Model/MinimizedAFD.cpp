#include "MinimizedAFD.hpp"

using namespace std;

bool MinimizedAFD::accepts(const string& input) const {
    int currentState = startState;
    
    // Processa cada símbolo da entrada
    for (char symbol : input) {
        int nextState = transitionOn(currentState, symbol);
        
        // Se não há transição, rejeita
        if (nextState == -1) {
            return false;
        }
        
        currentState = nextState;
    }
    
    // Aceita se termina em um estado de aceitação
    return acceptStates.find(currentState) != acceptStates.end();
}

int MinimizedAFD::transitionOn(int from, char symbol) const {
    // Valida o estado de origem
    if (from < 0 || from >= (int)states.size()) {
        return -1;
    }
    
    // Procura pela transição com o símbolo fornecido
    for (const auto& transition : states[from].transitions) {
        if (transition.first == symbol) {
            return transition.second;
        }
    }
    
    // Nenhuma transição encontrada
    return -1;
}

int MinimizedAFD::getOriginalState(int minimizedState) const {
    // Procura no mapa de classes de equivalência
    auto it = equivalenceClass.find(minimizedState);
    if (it != equivalenceClass.end()) {
        return it->second;
    }
    
    // Estado não encontrado no mapa
    return -1;
}

double MinimizedAFD::getCompressionRatio() const {
    // Retorna a razão entre o tamanho original e o minimizado
    // Estados únicos no mapa de equivalência = tamanho minimizado
    if (equivalenceClass.empty()) {
        return 0.0;
    }
    
    // Conta estados únicos no AFD original
    set<int> originalStates;
    for (const auto& pair : equivalenceClass) {
        originalStates.insert(pair.second);
    }
    
    int minimizedCount = equivalenceClass.size();
    int originalCount = originalStates.size();
    
    // Evita divisão por zero
    if (minimizedCount == 0) {
        return 0.0;
    }
    
    return (double)originalCount / minimizedCount;
}
