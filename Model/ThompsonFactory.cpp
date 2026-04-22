#include "ThompsonFactory.hpp"
#include "AFNDEpsilon.hpp"
#include "AFND.hpp"
#include "AFD.hpp"
#include "MinimizedAFD.hpp"
#include "Regex.hpp"
#include <stack>
#include <map>
#include <queue>

using namespace std;

shared_ptr<AFNDEpsilon> ThompsonFactory::thompsonSymbol(const string& symbol) {
    // Como a Factory é friend da classe, usamos o 'new' diretamente para 
    // contornar o construtor privado sem quebrar o encapsulamento.
    auto afnd = shared_ptr<AFNDEpsilon>(new AFNDEpsilon());
    
    int startState = afnd->addState();
    int acceptState = afnd->addState();
    
    afnd->setStartState(startState);
    afnd->addAcceptState(acceptState);
    
    // Se for um caractere simples ou um escape comum (ex: 'a' ou '\n')
    if (symbol.length() == 1 || (symbol.length() == 2 && symbol[0] == '\\')) {
        char c = (symbol.length() == 2) ? symbol[1] : symbol[0];
        afnd->addTransition(startState, c, acceptState);
    } 
    // Se for uma classe de caracteres como [a-z] ou [0-9]
    else if (symbol.length() > 2 && symbol.front() == '[' && symbol.back() == ']') {
        // Remove os colchetes para processar o conteúdo interno
        string content = symbol.substr(1, symbol.length() - 2);
        
        for (size_t i = 0; i < content.length(); ++i) {
            // Detecta um intervalo (ex: a-z)
            if (i + 2 < content.length() && content[i+1] == '-') {
                char startChar = content[i];
                char endChar = content[i+2];
                // Adiciona uma transição paralela para cada caractere no intervalo
                for (char c = startChar; c <= endChar; ++c) {
                    afnd->addTransition(startState, c, acceptState);
                }
                i += 2; // Pula o formato 'a-z'
            } else {
                // Caractere individual dentro da classe (ex: os símbolos em [+\-*/])
                // Ignora a barra de escape se ela estiver escapando algo dentro do colchete
                if (content[i] == '\\' && i + 1 < content.length()) {
                    i++;
                }
                afnd->addTransition(startState, content[i], acceptState);
            }
        }
    }
    
    return afnd;
}

shared_ptr<AFNDEpsilon> ThompsonFactory::thompsonConcat(
    shared_ptr<AFNDEpsilon> a,
    shared_ptr<AFNDEpsilon> b
) {
    // 1. Copia os estados de B para dentro de A
    int offsetB = a->appendStatesFrom(b);
    int startB = b->getStartState() + offsetB; // Novo ID do início de B
    
    // 2. Liga todos os antigos estados de aceitação de A no início de B com Epsilon
    for (int accState : a->getAcceptStates()) {
        a->addTransition(accState, '\0', startB);
    }
    
    // 3. Atualiza os estados de aceitação: A deixa de aceitar, apenas B aceita agora
    a->clearAcceptStates();
    for (int accStateB : b->getAcceptStates()) {
        a->addAcceptState(accStateB + offsetB);
    }
    
    return a;
}

shared_ptr<AFNDEpsilon> ThompsonFactory::thompsonUnion(
    shared_ptr<AFNDEpsilon> a,
    shared_ptr<AFNDEpsilon> b
) {
    auto result = shared_ptr<AFNDEpsilon>(new AFNDEpsilon());
    
    // 1. Cria novos estados inicial e final para o "diamante" da união
    int newStart = result->addState();
    int newAccept = result->addState();
    result->setStartState(newStart);
    result->addAcceptState(newAccept);
    
    // 2. Copia os autômatos A e B inteiros para o resultado
    int offsetA = result->appendStatesFrom(a);
    int offsetB = result->appendStatesFrom(b);
    
    // 3. Liga o novo início aos inícios de A e B ramificando com Epsilon (\0)
    result->addTransition(newStart, '\0', a->getStartState() + offsetA);
    result->addTransition(newStart, '\0', b->getStartState() + offsetB);
    
    // 4. Liga os antigos finais de A e B convergindo no novo final com Epsilon
    for (int accA : a->getAcceptStates()) {
        result->addTransition(accA + offsetA, '\0', newAccept);
    }
    for (int accB : b->getAcceptStates()) {
        result->addTransition(accB + offsetB, '\0', newAccept);
    }
    
    return result;
}

shared_ptr<AFNDEpsilon> ThompsonFactory::thompsonKleene(
    shared_ptr<AFNDEpsilon> a
) {
    int newStart = a->addState();
    int newAccept = a->addState();
    
    int oldStart = a->getStartState();
    
    // Epsilon do novo início para o antigo início (entrar no loop para 1+ ocorrências)
    a->addTransition(newStart, '\0', oldStart);
    // Epsilon do novo início direto pro final (permite zero ocorrências)
    a->addTransition(newStart, '\0', newAccept);
    
    for (int accA : a->getAcceptStates()) {
        // Epsilon do antigo final para o antigo início (repetir o loop indefinidamente)
        a->addTransition(accA, '\0', oldStart);
        // Epsilon do antigo final pro novo final (decidir sair do loop)
        a->addTransition(accA, '\0', newAccept);
    }
    
    a->setStartState(newStart);
    a->clearAcceptStates();
    a->addAcceptState(newAccept);
    
    return a;
}

shared_ptr<AFNDEpsilon> ThompsonFactory::generateAFNDEpsilon(const Regex& regexStructured) {
    stack<shared_ptr<AFNDEpsilon>> automatos;
    const vector<string>& rpn = regexStructured.getRPN();

    // Varre a Notação Polonesa Reversa
    for (const string& token : rpn) {
        if (token == "°") { // Operador de Concatenação explícito
            auto b = automatos.top(); automatos.pop();
            auto a = automatos.top(); automatos.pop();
            automatos.push(thompsonConcat(a, b));
        } 
        else if (token == "|") { // Operador de União (OU)
            auto b = automatos.top(); automatos.pop();
            auto a = automatos.top(); automatos.pop();
            automatos.push(thompsonUnion(a, b));
        } 
        else if (token == "*") { // Fecho de Kleene (Zero ou mais)
            auto a = automatos.top(); automatos.pop();
            automatos.push(thompsonKleene(a));
        }
        else if (token == "?") { // Opcional (Zero ou um) -> Equivalente a (A|ε)
            auto a = automatos.top(); automatos.pop();
            
            // Cria um autômato vazio que transita direto do início pro fim consumindo Epsilon
            auto vazio = shared_ptr<AFNDEpsilon>(new AFNDEpsilon());
            int s = vazio->addState(); int f = vazio->addState();
            vazio->setStartState(s); vazio->addAcceptState(f);
            vazio->addTransition(s, '\0', f);
            
            automatos.push(thompsonUnion(a, vazio));
        }
        else if (token == "+") { // Um ou mais -> Equivalente a A°A*
            auto a = automatos.top(); automatos.pop();
            
            // Instancia uma cópia de 'a' usando o construtor de cópia para não corromper os ponteiros
            auto aStar = thompsonKleene(shared_ptr<AFNDEpsilon>(new AFNDEpsilon(*a))); 
            automatos.push(thompsonConcat(a, aStar));
        }
        else {
            // Se não é operador, é um operando (letra, número ou classe [a-z]). Empilha.
            automatos.push(thompsonSymbol(token)); 
        }
    }
    
    // No final do processo da RPN, o único autômato que sobra na pilha é o grafo completo
    return automatos.empty() ? nullptr : automatos.top();
}

shared_ptr<AFND> ThompsonFactory::generateAFND(const shared_ptr<AFNDEpsilon>& nfaE) {
    if (!nfaE) return nullptr;
    auto nfa = shared_ptr<AFND>(new AFND());

    // 1. O novo estado inicial é o epsilon-closure do antigo estado inicial
    set<int> startSet = nfaE->epsilonClosure({nfaE->getStartState()});
    
    // 2. Simplificação estrutural: mantemos a mesma quantidade e IDs de estados
    // A lógica será criar transições diretas: i --a--> closure(transOn(i, a))
    for (int i = 0; i < nfaE->getStateCount(); ++i) {
        nfa->addState();
    }
    
    // Extrai o alfabeto real do autômato (todos os caracteres consumidos, exceto \0)
    set<char> alphabet = nfaE->getAlphabet(); 

    // Para cada estado possível no autômato original
    for (int i = 0; i < nfaE->getStateCount(); ++i) {
        // Encontra tudo que é alcançável a partir de 'i' usando apenas transições vazias
        set<int> closureI = nfaE->epsilonClosure({i});
        
        // Se o closure de 'i' atinge um estado de aceitação, então 'i' também vira aceitação
        if (nfaE->containsAcceptState(closureI)) {
            nfa->addAcceptState(i);
        }

        // Para cada símbolo do alfabeto, verifica para onde o closure consegue ir
        for (char c : alphabet) {
            // transitionOn() já retorna o epsilon-closure dos estados alcançáveis
            set<int> targets = nfaE->transitionOn(closureI, c);
            // Adiciona transições diretas bypassando os Epsilons intermediários
            for (int t : targets) {
                nfa->addTransition(i, c, t);
            }
        }
    }
    
    // Valida antes de definir o estado inicial
    int startState = nfaE->getStartState();
    if (startState < 0) {
        return nullptr;  // Estado inicial inválido
    }
    nfa->setStartState(startState);
    return nfa;
}

shared_ptr<AFD> ThompsonFactory::generateAFD(const shared_ptr<AFND>& afnd) {
    if (!afnd) return nullptr;
    auto afd = shared_ptr<AFD>(new AFD());
    
    // Alfabeto do autômato
    set<char> alphabet = afnd->getAlphabet();
    
    // Mapeamento: Conjunto de estados do AFND -> ID do estado no AFD
    map<set<int>, int> conjuntoParaId;
    queue<set<int>> pendentes;

    // 1. Estado inicial do AFD é o conjunto contendo o inicial do AFND
    set<int> inicialAFND = { afnd->getStartState() };
    int idInicial = afd->addState();
    afd->setStartState(idInicial);
    
    conjuntoParaId[inicialAFND] = idInicial;
    afd->stateMapping[idInicial] = inicialAFND; // Guarda mapeamento para debug
    pendentes.push(inicialAFND);

    while (!pendentes.empty()) {
        set<int> conjuntoAtual = pendentes.front();
        pendentes.pop();
        int idAtual = conjuntoParaId[conjuntoAtual];

        // Verifica se este conjunto contém algum estado de aceitação do AFND
        if (afnd->containsAcceptState(conjuntoAtual)) {
            afd->addAcceptState(idAtual);
        }

        // Para cada símbolo, descobre para onde o conjunto vai
        for (char c : alphabet) {
            set<int> proximoConjunto = afnd->transitionOn(conjuntoAtual, c);
            if (proximoConjunto.empty()) continue;

            // Se for um conjunto novo, cria um novo estado no AFD
            if (conjuntoParaId.find(proximoConjunto) == conjuntoParaId.end()) {
                int novoId = afd->addState();
                conjuntoParaId[proximoConjunto] = novoId;
                afd->stateMapping[novoId] = proximoConjunto;
                pendentes.push(proximoConjunto);
            }

            afd->addTransition(idAtual, c, conjuntoParaId[proximoConjunto]);
        }
    }
    return afd;
}

shared_ptr<MinimizedAFD> ThompsonFactory::minimizeAFD(const shared_ptr<AFD>& afd) {
    if (!afd) return nullptr;
    int n = afd->getStateCount();
    if (n == 0) return nullptr;

    // 1. Partição inicial (Aceitação vs Não-Aceitação)
    vector<int> grupo(n);
    for (int i = 0; i < n; i++) {
        grupo[i] = afd->getAcceptStates().count(i) ? 1 : 0;
    }

    set<char> alphabet = afd->getAlphabet(); // USANDO O MÉTODO DA BASE

    // 2. Refinamento de Grupos (Algoritmo de Moore)
    bool mudou = true;
    while (mudou) {
        mudou = false;
        map<pair<int, map<char, int>>, int> novosGrupos;
        vector<int> proximoGrupo(n);
        int contador = 0;

        for (int i = 0; i < n; i++) {
            map<char, int> transicoes;
            for (char c : alphabet) {
                int dest = afd->transitionOn(i, c);
                transicoes[c] = (dest == -1) ? -1 : grupo[dest];
            }
            
            auto chave = make_pair(grupo[i], transicoes);
            if (novosGrupos.find(chave) == novosGrupos.end()) {
                novosGrupos[chave] = contador++;
            }
            proximoGrupo[i] = novosGrupos[chave];
        }

        if (proximoGrupo != grupo) {
            grupo = proximoGrupo;
            mudou = true;
        }
    }

    // 3. Construção do Minimizador
    auto minAfd = shared_ptr<MinimizedAFD>(new MinimizedAFD());
    map<int, int> grupoParaNovoEstado;
    set<int> gruposProcessados;

    // Primeiro cria os estados únicos para cada grupo
    for (int i = 0; i < n; i++) {
        int g = grupo[i];
        if (grupoParaNovoEstado.find(g) == grupoParaNovoEstado.end()) {
            int novoId = minAfd->addState();
            grupoParaNovoEstado[g] = novoId;
            minAfd->equivalenceClass[novoId] = i; 
        }
    }

    // Define transições, inicial e finais usando um representante de cada grupo
    for (int i = 0; i < n; i++) {
        int g = grupo[i];
        if (gruposProcessados.count(g)) continue;
        gruposProcessados.insert(g);

        int de = grupoParaNovoEstado[g];
        
        // Se algum estado do grupo é inicial/final, o novo estado também é
        if (i == afd->getStartState()) minAfd->setStartState(de);
        if (afd->getAcceptStates().count(i)) minAfd->addAcceptState(de);

        for (char c : alphabet) {
            int destOriginal = afd->transitionOn(i, c);
            if (destOriginal != -1) {
                minAfd->addTransition(de, c, grupoParaNovoEstado[grupo[destOriginal]]);
            }
        }
    }
    return minAfd;
}