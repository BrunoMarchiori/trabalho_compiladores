#include "AFNDEpsilon.hpp"
#include <algorithm>
#include <queue>

using namespace std;

set<int> AFNDEpsilon::epsilonClosure(const set<int>& states) const {
    set<int> closure = states;
    queue<int> q;
    
    // Inicializa a fila com os estados fornecidos
    for (int state : states) {
        q.push(state);
    }
    
    // Busca para encontrar todos os estados alcançáveis por epsilon
    while (!q.empty()) {
        int current = q.front();
        q.pop();
        
        if (current < 0 || current >= (int)this->states.size()) continue;
        
        // Procura por transições epsilon (símbolo '\0') do estado atual
        for (const auto& transition : this->states[current].transitions) {
            char sym = transition.first;
            int target = transition.second;
            
            // Se é transição epsilon e o estado alvo não foi visitado ainda
            if (sym == '\0' && closure.find(target) == closure.end()) {
                closure.insert(target);
                q.push(target);
            }
        }
    }
    
    return closure;
}

set<int> AFNDEpsilon::transitionOn(
    const set<int>& states, 
    char symbol
) const {
    set<int> nextStates;
    
    // Para cada estado no conjunto
    for (int state : states) {
        if (state < 0 || state >= (int)this->states.size()) continue;
        
        // Procura transições com o símbolo fornecido
        for (const auto& transition : this->states[state].transitions) {
            if (transition.first == symbol) {
                nextStates.insert(transition.second);
            }
        }
    }
    
    // Calcula o epsilon-closure dos estados alcançáveis
    return epsilonClosure(nextStates);
}

bool AFNDEpsilon::containsAcceptState(const set<int>& states) const {
    // Verifica se algum estado no conjunto é um estado de aceitação
    for (int state : states) {
        if (this->acceptStates.find(state) != this->acceptStates.end()) {
            return true;
        }
    }
    return false;
}

bool AFNDEpsilon::accepts(const string& input) const {
    // Começa com o epsilon-closure do estado inicial
    set<int> currentStates;
    currentStates.insert(this->startState);
    currentStates = epsilonClosure(currentStates);
    
    // Processa cada símbolo da entrada
    for (char symbol : input) {
        currentStates = transitionOn(currentStates, symbol);
        
        // Se não há estados alcançáveis, rejeita
        if (currentStates.empty()) {
            return false;
        }
    }
    
    // Aceita se o conjunto final contém um estado de aceitação
    return containsAcceptState(currentStates);
}