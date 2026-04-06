#include <iostream>
#include <memory>
#include <vector>
#include <fstream>
#include <sstream>
#include "Controller/ScannerGenerator.hpp"
#include "Controller/Parser.hpp"
#include "Model/Token.h"

int main(int argc, char* argv[]) {
    std::cout << "========================================" << std::endl;
    std::cout << "  Compilador Racket - MVC Architecture  " << std::endl;
    std::cout << "========================================" << std::endl;
    
    // ========== 1. DETERMINAR ARQUIVO RACKET ==========
    std::string racketFilepath = "programa.rkt";
    
    if (argc > 1) {
        racketFilepath = argv[1];
    } else {
        std::cout << "\n[INFO] Uso: ./compilador_racket <arquivo.rkt>" << std::endl;
        std::cout << "[INFO] Usando arquivo padrão: programa.rkt" << std::endl;
    }
    
    // ========== 2. LER ARQUIVO RACKET ==========
    std::cout << "\n[Fase 1] Lendo arquivo Racket: " << racketFilepath << std::endl;
    
    std::ifstream racketFile(racketFilepath);
    if (!racketFile.is_open()) {
        std::cerr << "[ERRO] Não foi possível abrir arquivo '" 
                  << racketFilepath << "'" << std::endl;
        return 1;
    }
    
    std::stringstream buffer;
    buffer << racketFile.rdbuf();
    std::string code = buffer.str();
    racketFile.close();
    
    std::cout << "[OK] Arquivo lido (" << code.length() << " caracteres)" << std::endl;
    
    // ========== 3. CARREGAR REGRAS LÉXICAS ==========
    std::cout << "\n[Fase 2] Carregando regras léxicas (regex.txt)" << std::endl;
    
    ScannerGenerator scanGen;
    auto scanner = scanGen.generateFromFile("regex.txt");
    
    if (!scanner) {
        std::cerr << "[ERRO] " << scanGen.getLastError() << std::endl;
        return 1;
    }
    
    std::cout << "[OK] Scanner gerado com sucesso!" << std::endl;
    
    // ========== 4. LEXICALIZAR CÓDIGO ==========
    std::cout << "\n[Fase 3] Lexicalizando código Racket" << std::endl;
    
    std::vector<Token> tokens = scanner->scan(code);
    
    std::cout << "[OK] " << tokens.size() << " tokens gerados" << std::endl;
    
    // ========== 5. FAZER ANÁLISE SINTÁTICA ==========
    std::cout << "\n[Fase 4] Análise Sintática (Parser)" << std::endl;
    
    Parser parser(tokens);
    auto syntaxTree = parser.parse();
    
    if (!syntaxTree) {
        std::cerr << "[ERRO] " << parser.getLastError() << std::endl;
        return 1;
    }
    
    std::cout << "[OK] Árvore Sintática construída!" << std::endl;
    
    // ========== 6. EXIBIR RESULTADOS ==========
    std::cout << "\n[RESULTADO] Análise concluída com sucesso!" << std::endl;
    std::cout << "  - Tokens: " << tokens.size() << std::endl;
    std::cout << "  - Nós na AST: " << syntaxTree->getNodeCount() << std::endl;
    std::cout << "  - Profundidade: " << syntaxTree->getDepth() << std::endl;
    
    std::cout << "\n[DEBUG] Árvore Sintática:" << std::endl;
    std::cout << syntaxTree->toString() << std::endl;

    return 0;
}