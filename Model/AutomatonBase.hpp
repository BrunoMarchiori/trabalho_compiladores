#ifndef AUTOMATON_BASE_HPP
#define AUTOMATON_BASE_HPP

#include <vector>
#include <set>
#include <string>
#include <memory>
#include "State.hpp" // Certifique-se de que o State está incluído

class AutomatonBase {
protected:
    int startState;
    std::vector<State> states;
    std::set<int> acceptStates;
    std::string tokenType;

public:
    AutomatonBase() : startState(-1) {}
    virtual ~AutomatonBase() = default;

    // Métodos Genéricos de Gerenciamento do Grafo
    int addState() {
        int id = states.size();
        states.emplace_back(id);
        return id;
    }

    void addTransition(int from, char symbol, int to) {
        if (from >= 0 && from < (int)states.size()) {
            states[from].transitions.push_back({symbol, to});
        }
    }

    void setStartState(int id) { startState = id; }
    int getStartState() const { return startState; }
    
    void addAcceptState(int id) { acceptStates.insert(id); }
    const std::set<int>& getAcceptStates() const { return acceptStates; }
    void clearAcceptStates() { acceptStates.clear(); }
    
    int getStateCount() const { return (int)states.size(); }
    
    void setTokenType(const std::string& type) { tokenType = type; }
    std::string getTokenType() const { return tokenType; }
    
    // O método de merge inteligente que a Factory usa
    int appendStatesFrom(const std::shared_ptr<AutomatonBase>& other) {
        int offset = states.size();
        for (const auto& otherState : other->states) {
            int newId = this->addState();
            for (const auto& trans : otherState.transitions) {
                this->addTransition(newId, trans.first, trans.second + offset);
            }
        }
        return offset;
    }

    // Assinatura que as classes filhas são obrigadas a implementar
    virtual bool accepts(const std::string& input) const = 0;

    // Retorna o alfabeto usado pelo autômato (ignorando transições vazias)
    std::set<char> getAlphabet() const {
        std::set<char> alphabet;
        for (const State& state : states) {
            for (const auto& transition : state.transitions) {
                char symbol = transition.first;
                // Ignora o epsilon ('\0')
                if (symbol != '\0') {
                    alphabet.insert(symbol);
                }
            }
        }
        return alphabet;
    }
};

#endif // AUTOMATON_BASE_HPP