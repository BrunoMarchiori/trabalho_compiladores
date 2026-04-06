#ifndef SCANNER_GENERATOR_HPP
#define SCANNER_GENERATOR_HPP

#include "../Model/AutomatonFactory.hpp"
#include "../Model/Regex.hpp"
#include "../Model/Scanner.hpp"
#include <memory>
#include <vector>
#include <string>

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
    std::shared_ptr<AutomatonFactory> factory;
    std::string lastError;


    // Estrutura para armazenar regras parseadas
    struct RegexRule {
        std::string TokenType;
        std::string regexPattern;
    };
    
    // Faz parse do arquivo regex.txt
    bool parseRegexFile(const std::string& filepath, 
                       std::vector<RegexRule>& rules);
    
    // Valida uma linha do arquivo
    bool validateRegexLine(const std::string& line,
                          std::string& tokenType,
                          std::string& pattern);

    /**
     * Gera um autômato minimizado para uma única regra
     * executa: Regex → AFND-ε → AFND → AFD → AFD-Min
     */
    std::shared_ptr<MinimizedAFD> generateSingleScanner(
        const Regex& regex,
        const std::string& tokenType
    );
    
public:
    ScannerGenerator();
    
    // Recebe caminho do arquivo e retorna Scanner pronto
    std::shared_ptr<Scanner> generateFromFile(
        const std::string& regexFilepath
    );

    /**
     * Gera um scanner combinado para um conjunto de regras lexicais
     * 
     * @param rules Pares de (tokenType, expressão_regular)
     * @return Scanner pronto para usar, ou nullptr em caso de erro
     */
    std::shared_ptr<Scanner> generate(
        const std::vector<std::pair<std::string, Regex>>& rules
    );
    
    /**
     * Obtém a mensagem de erro da última operação
     */
    std::string getLastError() const;
};

#endif // SCANNER_GENERATOR_HPP
