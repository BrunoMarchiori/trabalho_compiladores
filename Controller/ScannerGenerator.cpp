#include "ScannerGenerator.hpp"
#include "../Model/ThompsonFactory.hpp"
#include "../Model/AFNDEpsilon.hpp"
#include "../Model/AFND.hpp"
#include "../Model/AFD.hpp"
#include "../Model/MinimizedAFD.hpp"
#include "../Model/Regex.hpp"
#include "../Model/Scanner.hpp"
#include <iostream>
#include <memory>

using namespace std;

ScannerGenerator::ScannerGenerator() 
    : factory(make_shared<ThompsonFactory>()) {}

shared_ptr<MinimizedAFD> ScannerGenerator::generateSingleScanner(
    const Regex& regex,
    const string& tokenType
) {
    try {
        // 1. Regex (RPN) → AFND-ε (Thompson Construction)
        auto afndEpsilon = factory->generateAFNDEpsilon(regex);
        if (!afndEpsilon) {
            lastError = "Falha ao gerar AFND-ε";
            return nullptr;
        }
        afndEpsilon->setTokenType(tokenType);
        
        // 2. AFND-ε → AFND (remover transições epsilon)
        auto afnd = factory->generateAFND(afndEpsilon);
        if (!afnd) {
            lastError = "Falha ao gerar AFND";
            return nullptr;
        }
        afnd->setTokenType(tokenType);
        
        // 3. AFND → AFD (subset construction)
        auto afd = factory->generateAFD(afnd);
        if (!afd) {
            lastError = "Falha ao gerar AFD";
            return nullptr;
        }
        afd->setTokenType(tokenType);
        
        // 4. AFD → AFD-Min (minimização)
        auto minimizedAfd = factory->minimizeAFD(afd);
        if (!minimizedAfd) {
            lastError = "Falha ao minimizar AFD";
            return nullptr;
        }
        minimizedAfd->setTokenType(tokenType);
        
        return minimizedAfd;
        
    } catch (const exception& e) {
        lastError = string("Exceção: ") + e.what();
        return nullptr;
    }
}

shared_ptr<Scanner> ScannerGenerator::generate(
    const vector<pair<string, Regex>>& rules
) {
    try {
        auto scanner = make_shared<Scanner>();
        
        for (const auto& rule : rules) {
            const string& tokenType = rule.first;
            const Regex& regex = rule.second;
            
            auto minimizedAfd = generateSingleScanner(regex, tokenType);
            if (!minimizedAfd) {
                lastError = "Falha ao gerar autômato para " + tokenType + ": " + lastError;
                return nullptr;
            }
            
            scanner->addRule(tokenType, minimizedAfd);
        }
        
        return scanner;
        
    } catch (const exception& e) {
        lastError = string("Exceção em generate(): ") + e.what();
        return nullptr;
    }
}

string ScannerGenerator::getLastError() const {
    return lastError;
}
