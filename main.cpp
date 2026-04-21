#include <iostream>
#include <fstream>  // <--- ADICIONE ESTA LINHA AQUI
#include <vector>
#include <string>
#include <iomanip>
#include "Model/Regex.hpp"
#include "Model/ThompsonFactory.hpp"
#include "Model/AFNDEpsilon.hpp"
#include "Model/AFND.hpp"
#include "Model/AFD.hpp"
#include "Model/MinimizedAFD.hpp"

using namespace std;

int main() {
    ThompsonFactory factory;
    
    // Agora o compilador sabe o que é um ifstream
    ifstream file("regex.txt");
    string line;

    // Verifica se o arquivo abriu corretamente para evitar crashes
    if (!file.is_open()) {
        cerr << "Erro: Não foi possível abrir o arquivo regex.txt" << endl;
        return 1;
    }

    cout << left << setw(25) << "TOKEN" << setw(10) << "NFA-e" << setw(10) << "NFA" 
         << setw(10) << "DFA" << setw(10) << "MIN" << endl;
    cout << string(65, '-') << endl;

    while (getline(file, line)) {
        if (line.empty() || line[0] == ';') continue;

        size_t start = line.find('<') + 1;
        size_t end = line.find('>');
        string type = line.substr(start, end - start);
        string pattern = line.substr(end + 2);

        Regex r(pattern);
        r.toRPN();

        auto nfaE = factory.generateAFNDEpsilon(r);
        auto nfa = factory.generateAFND(nfaE);
        auto afd = factory.generateAFD(nfa);
        auto min = factory.minimizeAFD(afd);

        // Algumas defesas caso algum ponteiro volte nulo por erro na regex
        int nfaE_count = nfaE ? nfaE->getStateCount() : 0;
        int nfa_count  = nfa ? nfa->getStateCount() : 0;
        int afd_count  = afd ? afd->getStateCount() : 0;
        int min_count  = min ? min->getStateCount() : 0;

        cout << left << setw(25) << type 
             << setw(10) << nfaE_count
             << setw(10) << nfa_count
             << setw(10) << afd_count
             << setw(10) << min_count << endl;
    }
    
    file.close();
    return 0;
}