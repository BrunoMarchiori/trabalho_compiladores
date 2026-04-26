#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <chrono>
#include <iomanip>
#include "Controller/ScannerGenerator.hpp"
#include "Controller/Parser.hpp"

using namespace std;

namespace {
struct CompilationMetrics {
    double loadRulesMs = 0.0;
    double buildScannerMs = 0.0;
    double readSourceMs = 0.0;
    double lexicalAnalysisMs = 0.0;
    double syntaxAnalysisMs = 0.0;
    double totalMs = 0.0;
    size_t ruleCount = 0;
    size_t tokenCount = 0;
    size_t lexicalErrorCount = 0;
    size_t syntaxErrorCount = 0;
};

double toMilliseconds(chrono::steady_clock::duration elapsed) {
    return chrono::duration<double, milli>(elapsed).count();
}

void printMetrics(const CompilationMetrics& metrics) {
    cout << "\nMétricas de compilação:\n";
    cout << fixed << setprecision(2);
    cout << "- Regras léxicas carregadas: " << metrics.ruleCount << '\n';
    cout << "- Tokens gerados: " << metrics.tokenCount << '\n';
    cout << "- Erros léxicos: " << metrics.lexicalErrorCount << '\n';
    cout << "- Erros sintáticos: " << metrics.syntaxErrorCount << '\n';
    cout << "- Tempo carregar regras: " << metrics.loadRulesMs << " ms\n";
    cout << "- Tempo gerar scanner: " << metrics.buildScannerMs << " ms\n";
    cout << "- Tempo ler fonte: " << metrics.readSourceMs << " ms\n";
    cout << "- Tempo análise léxica: " << metrics.lexicalAnalysisMs << " ms\n";
    cout << "- Tempo análise sintática: " << metrics.syntaxAnalysisMs << " ms\n";
    cout << "- Tempo total: " << metrics.totalMs << " ms\n";
}

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
    auto totalStart = chrono::steady_clock::now();
    CompilationMetrics metrics;

    const string regexPath = "regex.txt";
    const string sourcePath = (argc > 1) ? argv[1] : "programa.rkt";

    vector<pair<string, Regex>> rules;
    auto loadRulesStart = chrono::steady_clock::now();
    string errorMessage;
    if (!loadScannerRules(regexPath, rules, errorMessage)) {
        cerr << "Erro: " << errorMessage << endl;
        metrics.totalMs = toMilliseconds(chrono::steady_clock::now() - totalStart);
        printMetrics(metrics);
        return 1;
    }
    metrics.loadRulesMs = toMilliseconds(chrono::steady_clock::now() - loadRulesStart);
    metrics.ruleCount = rules.size();

    ScannerGenerator generator;
    auto buildScannerStart = chrono::steady_clock::now();
    auto scanner = generator.generate(rules);
    if (!scanner) {
        cerr << "Erro ao gerar scanner: " << generator.getLastError() << endl;
        metrics.totalMs = toMilliseconds(chrono::steady_clock::now() - totalStart);
        printMetrics(metrics);
        return 1;
    }
    metrics.buildScannerMs = toMilliseconds(chrono::steady_clock::now() - buildScannerStart);

    string sourceCode;
    auto readSourceStart = chrono::steady_clock::now();
    if (!readSourceFile(sourcePath, sourceCode, errorMessage)) {
        cerr << "Erro: " << errorMessage << endl;
        metrics.totalMs = toMilliseconds(chrono::steady_clock::now() - totalStart);
        printMetrics(metrics);
        return 1;
    }
    metrics.readSourceMs = toMilliseconds(chrono::steady_clock::now() - readSourceStart);

    auto lexicalStart = chrono::steady_clock::now();
    auto tokens = scanner->scan(sourceCode);
    metrics.lexicalAnalysisMs = toMilliseconds(chrono::steady_clock::now() - lexicalStart);
    metrics.tokenCount = tokens.size();

    vector<string> lexicalErrors;
    for (const auto& token : tokens) {
        if (token.type == TokenType::TOKEN_ERROR) {
            ostringstream oss;
            oss << "Linha " << token.line << ", Coluna " << token.column
                << ": símbolo inválido \"" << token.lexeme << "\"";
            lexicalErrors.push_back(oss.str());
        }
    }
    metrics.lexicalErrorCount = lexicalErrors.size();

    if (!lexicalErrors.empty()) {
        cout << "Programa rejeitado. Erros léxicos encontrados:\n";
        for (const auto& err : lexicalErrors) {
            cout << "- " << err << '\n';
        }
        metrics.totalMs = toMilliseconds(chrono::steady_clock::now() - totalStart);
        printMetrics(metrics);
        return 1;
    }

    Parser parser(tokens);
    auto syntaxStart = chrono::steady_clock::now();
    auto ast = parser.parse();
    metrics.syntaxAnalysisMs = toMilliseconds(chrono::steady_clock::now() - syntaxStart);
    metrics.syntaxErrorCount = parser.getErrors().size();

    if (parser.hasError() || !ast) {
        cout << "Programa rejeitado. Erros sintáticos encontrados:\n";
        for (const auto& err : parser.getErrors()) {
            cout << "- " << err << '\n';
        }
        metrics.totalMs = toMilliseconds(chrono::steady_clock::now() - totalStart);
        printMetrics(metrics);
        return 1;
    }

    cout << "Programa aceito.\n";
    cout << "AST:\n";
    cout << ast->toString() << '\n';
    metrics.totalMs = toMilliseconds(chrono::steady_clock::now() - totalStart);
    printMetrics(metrics);
    return 0;
}
