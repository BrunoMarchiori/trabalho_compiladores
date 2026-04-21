#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <memory>
#include "Model/Regex.hpp"
#include "Controller/ScannerGenerator.hpp"
#include "Controller/Parser.hpp"

using namespace std;

// Cores ANSI para terminal
const string RESET = "\033[0m";
const string BOLD = "\033[1m";
const string CYAN = "\033[36m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string MAGENTA = "\033[35m";

// Função para ler arquivo
string readFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Não conseguiu abrir arquivo: " + filename);
    }
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Função para parsear regex.txt
vector<pair<string, string>> parseRegexFile(const string& fileContent) {
    vector<pair<string, string>> rules;
    stringstream ss(fileContent);
    string line;
    
    while (getline(ss, line)) {
        // Ignora linhas vazias e comentários
        if (line.empty() || line[0] == ';') continue;
        
        // Formato: <TOKEN_TYPE> regex_pattern
        size_t openAngle = line.find('<');
        size_t closeAngle = line.find('>');
        
        if (openAngle == string::npos || closeAngle == string::npos) 
            continue;
        
        string tokenType = line.substr(openAngle + 1, closeAngle - openAngle - 1);
        string pattern = line.substr(closeAngle + 2); // Pula "> "
        
        rules.push_back({tokenType, pattern});
    }
    
    return rules;
}

// Função para exibir RPN de forma bonita
void printRPNTable(const vector<pair<string, string>>& rules) {
    cout << CYAN << "\n" << string(130, '=') << RESET << endl;
    cout << BOLD << CYAN << "  CONVERSÃO DE EXPRESSÕES REGULARES PARA RPN (Notação Polonesa Reversa)" << RESET << endl;
    cout << CYAN << string(130, '=') << RESET << endl;
    
    // Cabeçalho
    cout << BOLD 
         << setw(25) << left << "  TOKEN"
         << setw(40) << left << "  REGEX ORIGINAL"
         << setw(50) << left << "  RPN (Saída)"
         << RESET << endl;
    
    cout << CYAN << string(130, '-') << RESET << endl;
    
    // Tabela
    int count = 0;
    for (const auto& rule : rules) {
        const string& tokenType = rule.first;
        const string& pattern = rule.second;
        
        // Criar regex e converter para RPN
        Regex regex(pattern);
        regex.toRPN();
        
        // Construir string RPN
        string rpnStr = "";
        const auto& rpn = regex.getRPN();
        for (size_t i = 0; i < rpn.size(); i++) {
            rpnStr += rpn[i];
            if (i < rpn.size() - 1) rpnStr += " ";
        }
        
        // Cores alternadas para melhor visualização
        string rowColor = (count % 2 == 0) ? GREEN : YELLOW;
        
        cout << rowColor
             << setw(25) << left << ("  " + tokenType)
             << setw(40) << left << ("  " + pattern)
             << setw(50) << left << ("  " + rpnStr)
             << RESET << endl;
        
        count++;
    }
    
    cout << CYAN << string(130, '=') << RESET << endl << endl;
}

// Função para exibir detalhes de uma conversão
void printDetailedRPN(const string& tokenType, const string& pattern) {
    cout << BOLD << MAGENTA << "\n[DETALHES] " << tokenType << RESET << endl;
    cout << MAGENTA << string(80, '-') << RESET << endl;
    
    cout << "  Expressão Original: " << BOLD << pattern << RESET << endl;
    
    Regex regex(pattern);
    regex.toRPN();
    const auto& rpn = regex.getRPN();
    
    cout << "  RPN Gerada (" << rpn.size() << " elementos):" << endl;
    cout << "    ";
    for (size_t i = 0; i < rpn.size(); i++) {
        cout << GREEN << "[" << rpn[i] << "]";
        if (i < rpn.size() - 1) cout << " → ";
        RESET;
    }
    cout << RESET << endl;
    
    // Explicação do processamento
    cout << "\n  Processamento:" << endl;
    cout << "    • * e + têm precedência 3 (maior)" << endl;
    cout << "    • . (concatenação) têm precedência 2" << endl;
    cout << "    • | (união) têm precedência 1 (menor)" << endl;
    cout << MAGENTA << string(80, '-') << RESET << endl;
}

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    
    try {
        cout << "\n" << BOLD << CYAN << "+" << string(128, '=') << "+" << RESET << endl;
        cout << BOLD << CYAN << "|  COMPILADOR RACKET - Análise de Expressões Regulares" << string(71, ' ') << "|" << RESET << endl;
        cout << BOLD << CYAN << "+" << string(128, '=') << "+" << RESET << endl;
        
        // 1. LER REGEX.TXT
        cout << YELLOW << "\n[1] Carregando regras léxicas de regex.txt..." << RESET << endl;
        string regexFileContent = readFile("regex.txt");
        auto rules = parseRegexFile(regexFileContent);
        cout << GREEN << "    ✓ " << rules.size() << " regras carregadas com sucesso" << RESET << endl;
        
        // 2. EXIBIR TABELA COM TODAS AS RPN
        printRPNTable(rules);
        
        // 3. EXIBIR DETALHES DE ALGUNS EXEMPLOS
        cout << BOLD << MAGENTA << "EXEMPLOS DETALHADOS:" << RESET << endl;
        vector<string> exemplos = {"KEYWORD_DEFINE", "SYMBOL", "INTEGER", "FLOAT", "STRING"};
        
        for (const auto& example : exemplos) {
            for (const auto& rule : rules) {
                if (rule.first == example) {
                    printDetailedRPN(rule.first, rule.second);
                    break;
                }
            }
        }
        
        cout << GREEN << "\n[✓] Análise completa!" << RESET << endl;
        
    } catch (const exception& e) {
        cerr << "\n" << BOLD << "\033[31m[✗] ERRO: " << e.what() << RESET << endl;
        return 1;
    }
    
    return 0;
}