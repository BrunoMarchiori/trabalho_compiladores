#include <iostream>
#include <memory>
#include <vector>
#include "Controller/ScannerGenerator.hpp"
#include "Controller/Parser.hpp"
#include "Model/Regex.hpp"
#include "Model/Token.h"

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    
    std::cout << "========================================" << std::endl;
    std::cout << "  Compilador Racket - MVC Architecture  " << std::endl;
    std::cout << "========================================" << std::endl;
    
    // TODO: Implementar pipeline completo
    // 1. Carregar regras léxicas (regex.txt)
    // 2. Usar ScannerGenerator para criar Scanner via Abstract Factory
    // 3. Lexicalizar o código fonte
    // 4. Usar Parser para gerar Árvore Sintática
    // 5. Exibir resultados/erros
    
    std::cout << "\n[INFO] Estrutura MVC inicializada com sucesso!" << std::endl;
    std::cout << "  - Model: Autômatos (AFND-ε → AFND → AFD → AFD-Min)" << std::endl;
    std::cout << "  - Controller: ScannerGenerator + Parser" << std::endl;
    std::cout << "  - View: (a implementar)" << std::endl;
    
    std::cout << "\n[TODO] Próximos passos:" << std::endl;
    std::cout << "  1. Implementação de Thompson Construction" << std::endl;
    std::cout << "  2. Implementação de Epsilon Closure" << std::endl;
    std::cout << "  3. Implementação de Subset Construction (AFND→AFD)" << std::endl;
    std::cout << "  4. Implementação de Minimização (Hopcroft/Moore)" << std::endl;
    std::cout << "  5. Implementação do Parser Recursivo Descendente" << std::endl;
    std::cout << "  6. Testes com exemplos Racket" << std::endl;
    
    return 0;
}