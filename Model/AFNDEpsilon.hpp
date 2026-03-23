#ifndef AFND_EPSILON_HPP
#define AFND_EPSILON_HPP

#include "AutomatonBase.hpp"
#include "Regex.hpp"
#include <set>
#include <memory>

/**
 * AFND com transições epsilon (ε-NFA)
 * Permite transições vazias que mudam de estado sem consumir símbolo
 */
class AFNDEpsilon : public AutomatonBase {
private:
    friend class AutomatonFactory;
    friend class ThompsonFactory;  // Factory que implementa Thompson
    
    // Construtor protegido - apenas Factory pode criar
    AFNDEpsilon() = default;
    
public:
    /**
     * Computa o fecho epsilon (ε-closure) de um conjunto de estados
     * Retorna todos os estados alcançáveis apenas por transições epsilon
     */
    std::set<int> epsilonClosure(const std::set<int>& states) const;
    
    /**
     * Retorna todos os estados alcançáveis de 'states' consumindo 'symbol'
     * seguido de transições epsilon
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
     * Simula o AFND-ε com uma entrada
     */
    bool accepts(const std::string& input) const;
};

#endif // AFND_EPSILON_HPP