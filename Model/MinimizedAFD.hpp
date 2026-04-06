#ifndef MINIMIZED_AFD_HPP
#define MINIMIZED_AFD_HPP

#include "AutomatonBase.hpp"
#include <memory>
#include <map>

using namespace std;

// Forward declaration
class AFD;

/**
 * AFD Minimizado
 * Gerado a partir de um AFD através de agrupamento de estados equivalentes
 * Reduz o número de estados sem alterar a linguagem aceita
 */
class MinimizedAFD : public AutomatonBase {
private:
    friend class AutomatonFactory;
    friend class ThompsonFactory;
    
    // Mapeia cada estado minimizado para o estado original do AFD
    map<int, int> equivalenceClass;
    
    // Construtor protegido - apenas Factory pode criar
    MinimizedAFD() = default;
    
public:
    /**
     * Simula o AFD minimizado com uma entrada
     */
    bool accepts(const string& input) const;
    
    /**
     * Retorna o estado alcançável de 'from' consumindo 'symbol'
     */
    int transitionOn(int from, char symbol) const;
    
    /**
     * Obtém o estado AFD original que este estado representa
     */
    int getOriginalState(int minimizedState) const;
    
    /**
     * Retorna o fator de compressão (ratio entre AFD original e minimizado)
     */
    double getCompressionRatio() const;
};

#endif // MINIMIZED_AFD_HPP
