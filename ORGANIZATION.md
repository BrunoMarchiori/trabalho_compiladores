# Organização MVC - Compilador Racket

## Visão Geral

Este projeto segue o padrão **Model-View-Controller (MVC)** com uso de **Abstract Factory Pattern** para criação de autômatos.

### Pipeline Arquitetural

```
Regex (entrada)
    ↓
[Thompson Factory - Model]
    ↓
AFND-ε (NFA com epsilon)
    ↓
Epsilon Closure
    ↓
AFND (NFA sem epsilon)
    ↓
Subset Construction
    ↓
AFD (DFA)
    ↓
Minimização
    ↓
AFD-Min (Minimized DFA)
    ↓
[Scanner combina todos - Model]
    ↓
Tokens (saída para Parser)
    ↓
[Parser - Controller]
    ↓
AST (Syntax Tree)
    ↓
[View - Visualização]
```

## Estrutura MVC

### 🗂️ Model/ - Estruturas de Dados e Autômatos

**Hierarquia de Autômatos:**
```
AutomatonBase
├── AFNDEpsilon      ← AFND com transições epsilon
├── AFND             ← AFND sem epsilon
├── AFD              ← Autômato determinístico
└── MinimizedAFD     ← AFD minimizado
```

**Abstract Factory:**
```
AutomatonFactory (interface)
    └── ThompsonFactory (implementação)
        ├── generateAFNDEpsilon()    - Thompson Construction
        ├── generateAFND()           - Remove epsilon
        ├── generateAFD()            - Subset Construction
        └── minimizeAFD()            - Hopcroft/Moore
```

**Arquivos principais:**
- `State.hpp` - Estrutura de um estado em autômato
- `AutomatonBase.hpp` - Classe base para todos os autômatos
- `AutomatonFactory.hpp` - Interface da factory
- `ThompsonFactory.{hpp,cpp}` - Implementação Thompson
- `Regex.{hpp,cpp}` - Expressão regular estruturada (RPN)
- `Scanner.{hpp,cpp}` - Lexicalizador: combina múltiplos autômatos
- `Token.h` - Definição de Token

### 🎮 Controller/ - Orquestração e Análise

**Responsabilidades:**
- Orquestar a criação de scanners via factory
- Implementar análise sintática (parsing)
- Gerenciar representação da árvore sintática

**Arquivos principais:**
- `ScannerGenerator.{hpp,cpp}` - Orquestrador do pipeline Regex→Scanner
  - Usa Abstract Factory para criar autômatos
  - Retorna Scanner pronto para lexicalizar código
  
- `Parser.{hpp,cpp}` - Parser recursivo descendente para Racket
  - Métodos para cada forma especial (define, lambda, if, cond, let)
  - Gerenciamento de erros com sincronização
  
- `SyntaxTree.{hpp,cpp}` - Representação de AST
  - Diferentes tipos de nós (LITERAL, SYMBOL, FUNCTION_CALL, etc.)
  - Impressão e análise da árvore

### 👁️ View/ - Visualização (Futura)

Placeholder para:
- Visualização de árvore sintática
- Impressão de erros com contexto
- Debug de autômatos (visualização de estados)

### 📄 main.cpp

Ponto de entrada do programa. Atualmente demonstra:
- Inicialização da estrutura MVC
- Esboço de como usar ScannerGenerator e Parser

## Como Usar (Exemplo)

```cpp
// 1. Importar o gerador
#include "Controller/ScannerGenerator.hpp"

// 2. Criar regras léxicas (Regex + TokenType)
std::vector<std::pair<std::string, Regex>> rules = {
    {"DEFINE", Regex("define")},
    {"LPAREN", Regex("\\(")},
    {"RPAREN", Regex("\\)")},
    {"NUM", Regex("[0-9]+")},
};

// 3. Gerar scanner via factory
ScannerGenerator generator;
auto scanner = generator.generate(rules);

if (!scanner) {
    std::cerr << "Erro: " << generator.getLastError() << std::endl;
    return 1;
}

// 4. Lexicalizar código
std::string sourceCode = "(define x 42)";
auto tokens = scanner->scan(sourceCode);

// 5. Parsear tokens
Parser parser(tokens);
auto ast = parser.parse();

if (parser.hasError()) {
    std::cerr << "Erro sintático: " << parser.getLastError() << std::endl;
    return 1;
}

// 6. Usar a AST
std::cout << ast->toString() << std::endl;
```

## Compilação

```bash
# Limpar compilação anterior
make clean

# Compilar projeto
make

# Executar
./compilador_racket
```

## Arquivos de Configuração

- **regex.txt** - Define as regras léxicas do Racket
  - Formato: `<TOKENTYPE> pattern`
  - Exemplo: `<IF> if`

- **Makefile** - Sistema de build
  - Agrupa arquivos por camada MVC
  - Compila com flags de segurança (-Wall -Wextra)

## Próximas Implementações

1. **Thompson Construction** - Gerar AFND-ε a partir de expressão regular
2. **Epsilon Closure** - Remover transições epsilon
3. **Subset Construction** - Converter AFND para AFD
4. **Minimização** - Reduzir número de estados do AFD
5. **Lexicalização** - Escanear código fonte
6. **Parser Completo** - Todas as formas Racket (define, lambda, if, etc.)

## Padrões de Design Utilizados

- **Abstract Factory** - Criação modular de autômatos
- **Strategy** - Diferentes operações Thompson (union, concat, kleene)
- **Composite** - Árvore sintática
- **Model-View-Controller** - Separação de responsabilidades
