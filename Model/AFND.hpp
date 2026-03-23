#ifndef AFND_HPP
#define AFND_HPP

#include "AutomatonBase.hpp"
#include <memory>

// Forward declaration
class AFNDEpsilon;

/**
 * AFND (NFA - Nondeterministic Finite Automaton)
 * Versão sem transições epsilon
 * Cada transição leva a um único estado definido
 */
class AFND : public AutomatonBase {
private:
    friend class AutomatonFactory;
    friend class ThompsonFactory;
    
    // Construtor protegido - apenas Factory pode criar
    AFND() = default;
    
public:
    /**
     * Retorna todos os estados alcançáveis de 'states' consumindo 'symbol'
     */
    std::set<int> transitionOn(
        const std::set<int>& states, 
        char symbol
    ) const;
    
    /**
     * Verifica se um conjunto de estados contém um estado de aceitação
     */
    bool containsAcceptState(const std::set<int>& states) const;
    
    /**
     * Simula o AFND com uma entrada
     */
    bool accepts(const std::string& input) const;
    
    /**
     * Obtém o alfabeto (símbolos) usados neste autômato
     */
    std::set<char> getAlphabet() const;
};

#endif // AFND_HPP
