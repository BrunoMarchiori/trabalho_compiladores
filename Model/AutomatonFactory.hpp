#ifndef AUTOMATON_FACTORY_HPP
#define AUTOMATON_FACTORY_HPP

#include "State.hpp"
#include "Regex.hpp"
#include <vector>
#include <memory>

// Forward declarations
class AFNDEpsilon;
class AFND;
class AFD;
class MinimizedAFD;

/**
 * Abstract Factory para gerar autômatos progressivamente
 * Pipeline: Regex → AFND-ε → AFND → AFD → AFD-Min
 * 
 * O padrão Factory encapsula a criação complexa de autômatos,
 * permitindo gerenciar o contador de estados e as transições de forma centralizada
 */
class AutomatonFactory {
protected:
    int stateCounter = 0;
    
    /**
     * Cria um novo estado único
     */
    virtual State createState();
    
public:
    virtual ~AutomatonFactory() = default;
    
    /**
     * Gera AFND-ε a partir de uma expressão regular estruturada
     * (em notação polonesa reversa)
     */
    virtual std::shared_ptr<AFNDEpsilon> generateAFNDEpsilon(
        const Regex& regexStructured
    ) = 0;
    
    /**
     * Converte AFND-ε para AFND (remove transições epsilon)
     */
    virtual std::shared_ptr<AFND> generateAFND(
        const std::shared_ptr<AFNDEpsilon>& afndEpsilon
    ) = 0;
    
    /**
     * Converte AFND para AFD via subconjunto (subset construction)
     */
    virtual std::shared_ptr<AFD> generateAFD(
        const std::shared_ptr<AFND>& afnd
    ) = 0;
    
    /**
     * Minimiza um AFD usando algoritmo de Hopcroft ou Moore
     */
    virtual std::shared_ptr<MinimizedAFD> minimizeAFD(
        const std::shared_ptr<AFD>& afd
    ) = 0;
};

#endif // AUTOMATON_FACTORY_HPP
