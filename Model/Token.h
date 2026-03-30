#ifndef TOKEN_H
#define TOKEN_H

#include <string>

using namespace std;

// Enumeração útil para o Parser identificar os blocos principais
enum class TokenType {
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_IDENTIFIER,
    TOKEN_WHITESPACE,
    TOKEN_EOF,      // Fim do arquivo
    TOKEN_ERROR,    // Para quando o scanner achar algo inválido
    TOKEN_DYNAMIC   // Para os tipos lidos do arquivo de regex
};

struct Token {
    TokenType type;
    string typeName; // Ex: "IDENTIFIER", "NUMBER" (útil para debug e impressão)
    string lexeme;   // O texto real, ex: "define", "(", "42"
    int line;
    int column;
};

#endif