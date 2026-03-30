#include <iostream>
#include <memory>
#include <vector>
#include "Controller/ScannerGenerator.hpp"
#include "Controller/Parser.hpp"
#include "Model/Regex.hpp"
#include "Model/Token.h"

using namespace std;

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    
    cout << "========================================" << endl;
    cout << "  Compilador Racket - MVC Architecture  " << endl;
    cout << "========================================" << endl;
    
    // TODO: Implementar pipeline completo
    // 1. Carregar regras léxicas (regex.txt)
    // 2. Usar ScannerGenerator para criar Scanner via Abstract Factory
    // 3. Lexicalizar o código fonte
    // 4. Usar Parser para gerar Árvore Sintática
    // 5. Exibir resultados/erros
    
    cout << "\n[INFO] Estrutura MVC inicializada com sucesso!" << endl;
    cout << "  - Model: Autômatos (AFND-ε → AFND → AFD → AFD-Min)" << endl;
    cout << "  - Controller: ScannerGenerator + Parser" << endl;
    cout << "  - View: (a implementar)" << endl;
    
    cout << "\n[TODO] Próximos passos:" << endl;
    cout << "  1. Implementação de Thompson Construction" << endl;
    cout << "  2. Implementação de Epsilon Closure" << endl;
    cout << "  3. Implementação de Subset Construction (AFND→AFD)" << endl;
    cout << "  4. Implementação de Minimização (Hopcroft/Moore)" << endl;
    cout << "  5. Implementação do Parser Recursivo Descendente" << endl;
    cout << "  6. Testes com exemplos Racket" << endl;
    
    return 0;
}