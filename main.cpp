#include <iostream>
#include <memory>
#include <vector>
#include <fstream>
#include <sstream>
#include "Controller/ScannerGenerator.hpp"
#include "Controller/Parser.hpp"
#include "Model/Regex.hpp"
#include "Model/Token.h"

using namespace std;

// Função helper para ler arquivo
string readFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Não conseguiu abrir arquivo: " + filename);
    }
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Função helper para parsear regex.txt
vector<pair<string, Regex>> parseRegexFile(const string& fileContent) {
    vector<pair<string, Regex>> rules;
    stringstream ss(fileContent);
    string line;
    
    while (getline(ss, line)) {
        // Ignora linhas vazias e comentários
        if (line.empty() || line[0] == ';') continue;
        
        // Formato: <TOKEN_TYPE> regex_pattern
        size_t pos = line.find('>');
        if (pos == string::npos) continue;
        
        string tokenType = line.substr(1, pos - 1);  // Remove < >
        string pattern = line.substr(pos + 2);       // Remove espaço
        
        Regex regex(pattern);
        regex.setTokenType(tokenType);
        rules.push_back({tokenType, regex});
    }
    
    return rules;
}

int main(int argc, char* argv[]) {
    cout << "========================================" << endl;
    cout << "  Compilador Racket - MVC Architecture  " << endl;
    cout << "========================================" << endl;
    
    try {
        // 1. LER REGEX.TXT
        cout << "\n[1] Carregando regras léxicas..." << endl;
        string regexFileContent = readFile("regex.txt");
        vector<pair<string, Regex>> rules = parseRegexFile(regexFileContent);
        cout << "    ✓ " << rules.size() << " regras carregadas" << endl;
        
        // 2. GERAR SCANNER (ScannerGenerator)
        cout << "\n[2] Gerando scanner..." << endl;
        ScannerGenerator generator;
        auto scanner = generator.generate(rules);
        
        if (!scanner) {
            cerr << "✗ Erro ao gerar scanner: " << generator.getLastError() << endl;
            return 1;
        }
        cout << "    ✓ Scanner gerado com sucesso" << endl;
        
        // 3. LER PROGRAMA.RKT
        cout << "\n[3] Lendo programa..." << endl;
        string programContent = readFile("programa.rkt");
        
        // 4. LEXICALIZAR
        cout << "\n[4] Lexicalizando..." << endl;
        auto tokens = scanner->scan(programContent);
        cout << "    ✓ " << tokens.size() << " tokens gerados" << endl;
        
        // 5. FAZER PARSING
        cout << "\n[5] Parsing..." << endl;
        Parser parser(tokens);
        auto ast = parser.parse();
        
        if (!ast) {
            cerr << "✗ Erro de parsing" << endl;
            return 1;
        }
        
        // 6. EXIBIR AST
        cout << "\n[6] Árvore Sintática:" << endl;
        cout << ast->toString() << endl;
        
    } catch (const exception& e) {
        cerr << "✗ Erro: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}