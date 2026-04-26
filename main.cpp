#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "Controller/ScannerGenerator.hpp"
#include "Controller/Parser.hpp"

using namespace std;

namespace {
bool parseRegexRule(const string& line, string& tokenType, string& pattern) {
    if (line.empty() || line[0] == ';') {
        return false;
    }

    size_t start = line.find('<');
    size_t end = line.find('>');
    if (start == string::npos || end == string::npos || end <= start + 1) {
        return false;
    }

    tokenType = line.substr(start + 1, end - start - 1);
    pattern = line.substr(end + 1);

    size_t firstNonSpace = pattern.find_first_not_of(" \t\r");
    if (firstNonSpace == string::npos) {
        return false;
    }
    pattern = pattern.substr(firstNonSpace);
    return true;
}

bool loadScannerRules(const string& regexPath, vector<pair<string, Regex>>& rules, string& err) {
    ifstream file(regexPath);
    if (!file.is_open()) {
        err = "Não foi possível abrir o arquivo de regex: " + regexPath;
        return false;
    }

    string line;
    while (getline(file, line)) {
        string tokenType;
        string pattern;
        if (!parseRegexRule(line, tokenType, pattern)) {
            continue;
        }

        Regex regex(pattern);
        regex.toRPN();
        rules.push_back({tokenType, regex});
    }

    if (rules.empty()) {
        err = "Nenhuma regra válida encontrada em " + regexPath;
        return false;
    }

    return true;
}

bool readSourceFile(const string& sourcePath, string& sourceCode, string& err) {
    ifstream sourceFile(sourcePath);
    if (!sourceFile.is_open()) {
        err = "Não foi possível abrir o arquivo fonte: " + sourcePath;
        return false;
    }

    ostringstream oss;
    oss << sourceFile.rdbuf();
    sourceCode = oss.str();
    return true;
}
} // namespace

int main(int argc, char* argv[]) {
    const string regexPath = "regex.txt";
    const string sourcePath = (argc > 1) ? argv[1] : "programa.rkt";

    vector<pair<string, Regex>> rules;
    string errorMessage;
    if (!loadScannerRules(regexPath, rules, errorMessage)) {
        cerr << "Erro: " << errorMessage << endl;
        return 1;
    }

    ScannerGenerator generator;
    auto scanner = generator.generate(rules);
    if (!scanner) {
        cerr << "Erro ao gerar scanner: " << generator.getLastError() << endl;
        return 1;
    }

    string sourceCode;
    if (!readSourceFile(sourcePath, sourceCode, errorMessage)) {
        cerr << "Erro: " << errorMessage << endl;
        return 1;
    }

    auto tokens = scanner->scan(sourceCode);
    vector<string> lexicalErrors;
    for (const auto& token : tokens) {
        if (token.type == TokenType::TOKEN_ERROR) {
            ostringstream oss;
            oss << "Linha " << token.line << ", Coluna " << token.column
                << ": símbolo inválido \"" << token.lexeme << "\"";
            lexicalErrors.push_back(oss.str());
        }
    }

    if (!lexicalErrors.empty()) {
        cout << "Programa rejeitado. Erros léxicos encontrados:\n";
        for (const auto& err : lexicalErrors) {
            cout << "- " << err << '\n';
        }
        return 1;
    }

    Parser parser(tokens);
    auto ast = parser.parse();

    if (parser.hasError() || !ast) {
        cout << "Programa rejeitado. Erros sintáticos encontrados:\n";
        for (const auto& err : parser.getErrors()) {
            cout << "- " << err << '\n';
        }
        return 1;
    }

    cout << "Programa aceito.\n";
    cout << "AST:\n";
    cout << ast->toString() << '\n';
    return 0;
}
