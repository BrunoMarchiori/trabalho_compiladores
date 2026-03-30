#ifndef AFD_HPP
#define AFD_HPP

#include "AutomatonBase.hpp"
#include <memory>
#include <map>
#include <set>

using namespace std;

// Forward declaration
class AFND;

/**
 * AFD (DFA - Deterministic Finite Automaton)
 * Gerado a partir de um AFND via subset construction
 * Cada estado representa um conjunto de estados do AFND
 */
class AFD : public AutomatonBase {
private:
    friend class AutomatonFactory;
    friend class ThompsonFactory;
    
    // Mapeia cada estado para o conjunto de AFND-states que representa
    map<int, set<int>> stateMapping;
    
    // Construtor protegido - apenas Factory pode criar
    AFD() = default;
    
public:
    /**
     * Simula o AFD com uma entrada
     * Determinístico: para cada símbolo, há no máximo uma transição
     */
    bool accepts(const string& input) const;
    
    /**
     * Retorna o estado alcançável de 'from' consumindo 'symbol'
     * Retorna -1 se não houver transição definida
     */
    int transitionOn(int from, char symbol) const;
    
    /**
     * Obtém o conjunto de AFND-states que o estado AFD representa
     */
    set<int> getMapping(int afdState) const;
    
    /**
     * Verifica se o autômato está completamente definido
     * (toda transição tem um destino, sem "dead states" implícitos)
     */
    bool isComplete() const;
};

#endif // AFD_HPP
