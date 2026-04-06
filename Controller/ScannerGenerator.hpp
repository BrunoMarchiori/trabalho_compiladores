#ifndef SCANNER_GENERATOR_HPP
#define SCANNER_GENERATOR_HPP

#include "../Model/AutomatonFactory.hpp"
#include "../Model/Regex.hpp"
#include "../Model/Scanner.hpp"
#include <memory>
#include <vector>
#include <string>

using namespace std;

/**
 * Controller: Gerador de Scanners
 * 
 * Orquestra o pipeline completo de Scanner:
 * 1. Recebe regras (pares de tokenType e expressão regular)
 * 2. Usa a Abstract Factory para gerar autômatos progressivamente
 * 3. Retorna um Scanner combinado pronto para lexicalizar código
 */
class ScannerGenerator {
private:
    shared_ptr<AutomatonFactory> factory;
    string lastError;
    
    /**
     * Gera um autômato minimizado para uma única regra
     * executa: Regex → AFND-ε → AFND → AFD → AFD-Min
     */
    shared_ptr<MinimizedAFD> generateSingleScanner(
        const Regex& regex,
        const string& tokenType
    );
    
public:
    ScannerGenerator();
    
    /**
     * Gera um scanner combinado para um conjunto de regras lexicais
     * 
     * @param rules Pares de (tokenType, expressão_regular)
     * @return Scanner pronto para usar, ou nullptr em caso de erro
     */
    shared_ptr<Scanner> generate(
        const vector<pair<string, Regex>>& rules
    );
    
    /**
     * Obtém a mensagem de erro da última operação
     */
    string getLastError() const;
};

#endif // SCANNER_GENERATOR_HPP
