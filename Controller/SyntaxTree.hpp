#ifndef SYNTAX_TREE_HPP
#define SYNTAX_TREE_HPP

#include <string>
#include <vector>
#include <memory>

using namespace std;

/**
 * Nó da Árvore Sintática
 * Representa um elemento da linguagem Racket
 */
class SyntaxTree {
public:
    enum class NodeType {
        PROGRAM,           // Raiz: múltiplas expressões
        LITERAL_NUM,       // Número: 42, 3.14
        LITERAL_BOOL,      // Booleano: #t, #f
        LITERAL_STRING,    // String: "hello"
        SYMBOL,            // Símbolo: x, +, define
        DEFINE,            // Forma especial: (define ...)
        LAMBDA,            // Forma especial: (lambda ...)
        IF,                // Forma especial: (if ...)
        COND,              // Forma especial: (cond ...)
        LET,               // Forma especial: (let ...)
        FUNCTION_CALL,     // Aplicação: (func arg1 arg2 ...)
        BINDING_LIST,      // [(var expr) ...] em let
        COND_CLAUSE,       // [test expr] em cond
        QUOTE,             // Citação: 'expr
        LIST               // Lista: (a b c)
    };
    
    NodeType type;
    string value;                      // Valor do nó (para LITERAL_*, SYMBOL)
    vector<shared_ptr<SyntaxTree>> children;
    
    explicit SyntaxTree(NodeType nodeType, const string& nodeValue = "")
        : type(nodeType), value(nodeValue) {}
    
    /**
     * Adiciona um filho a este nó
     */
    void addChild(shared_ptr<SyntaxTree> child) {
        children.push_back(child);
    }
    
    /**
     * Converte a árvore para string (para debug/visualização)
     */
    string toString(int depth = 0) const;
    
    /**
     * Retorna o número de nós na árvore
     */
    int getNodeCount() const;
    
    /**
     * Retorna a profundidade da árvore
     */
    int getDepth() const;
};

#endif // SYNTAX_TREE_HPP
