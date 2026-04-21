#ifndef THOMPSON_FACTORY_HPP
#define THOMPSON_FACTORY_HPP

#include "AutomatonFactory.hpp"
#include "State.hpp"
#include <stack>
#include <vector>
#include <memory>

using namespace std;

// Forward declarations para evitar includes circulares
class Regex;
class AFNDEpsilon;
class AFND;
class AFD;
class MinimizedAFD;

/**
 * Implementação concreta da Abstract Factory usando Construção de Thompson
 * 
 * Pipeline:
 * 1. Regex (notação polonesa reversa) → AFND-ε usando Thompson
 * 2. AFND-ε → AFND (remoção de epsilon-transitions via epsilon-closure)
 * 3. AFND → AFD (subset construction)
 * 4. AFD → AFD-Min (agrupamento de estados equivalentes via Hopcroft/Moore)
 */
class ThompsonFactory : public AutomatonFactory {
private:
    /**
     * Operações base de Thompson para construir AFND-ε
     */
    
    //// Cria AFND-ε para um símbolo literal ou classe de caracteres
    shared_ptr<AFNDEpsilon> thompsonSymbol(const string& symbol);
    
    /// Concatena dois AFND-ε: a seguido de b
    shared_ptr<AFNDEpsilon> thompsonConcat(
        shared_ptr<AFNDEpsilon> a,
        shared_ptr<AFNDEpsilon> b
    );
    
    /// União de dois AFND-ε: a | b
    shared_ptr<AFNDEpsilon> thompsonUnion(
        shared_ptr<AFNDEpsilon> a,
        shared_ptr<AFNDEpsilon> b
    );
    
    /// Fecho de Kleene: a*
    shared_ptr<AFNDEpsilon> thompsonKleene(
        shared_ptr<AFNDEpsilon> a
    );
    
public:
    ThompsonFactory() = default;
    
    /**
     * Gera AFND-ε a partir de Regex (RPN) usando Construção de Thompson
     * Processa a notação polonesa reversa com uma pilha
     */
    shared_ptr<AFNDEpsilon> generateAFNDEpsilon(
        const Regex& regexStructured
    ) override;
    
    /**
     * Remove transições epsilon via epsilon-closure
     */
    shared_ptr<AFND> generateAFND(
        const shared_ptr<AFNDEpsilon>& afndEpsilon
    ) override;
    
    /**
     * Subset construction: converte AFND em AFD
     */
    shared_ptr<AFD> generateAFD(
        const shared_ptr<AFND>& afnd
    ) override;
    
    /**
     * Minimiza AFD via agrupamento de estados equivalentes
     */
    shared_ptr<MinimizedAFD> minimizeAFD(
        const shared_ptr<AFD>& afd
    ) override;
};

#endif // THOMPSON_FACTORY_HPP
