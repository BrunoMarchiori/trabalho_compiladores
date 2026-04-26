#include "Scanner.hpp"
#include <algorithm>

using namespace std;

namespace {
Token makeStringToken(const string& lexeme, int line, int column) {
    Token token;
    token.type = TokenType::TOKEN_STRING;
    token.typeName = "STRING";
    token.lexeme = lexeme;
    token.line = line;
    token.column = column;
    return token;
}
} // namespace

Scanner::Scanner(shared_ptr<MinimizedAFD> singleRule) {
    if (singleRule) {
        rules.push_back({"DEFAULT", singleRule});
    }
}

void Scanner::addRule(
    const string& tokenType, 
    shared_ptr<MinimizedAFD> automaton
) {
    if (automaton) {
        rules.push_back({tokenType, automaton});
    }
}

vector<Token> Scanner::scan(const string& sourceCode) {
    vector<Token> tokens;
    int line = 1;
    int column = 1;
    size_t pos = 0;
    
    while (pos < sourceCode.length()) {
        // Processa strings diretamente para suportar escapes e conteúdo UTF-8.
        if (sourceCode[pos] == '"') {
            size_t startPos = pos;
            int startLine = line;
            int startColumn = column;
            pos++;
            column++;

            bool escaped = false;
            bool closed = false;

            while (pos < sourceCode.length()) {
                char c = sourceCode[pos];
                if (c == '\n') {
                    line++;
                    column = 1;
                } else {
                    column++;
                }

                if (!escaped && c == '"') {
                    pos++;
                    closed = true;
                    break;
                }

                if (!escaped && c == '\\') {
                    escaped = true;
                } else {
                    escaped = false;
                }

                pos++;
            }

            Token token;
            if (closed) {
                token = makeStringToken(sourceCode.substr(startPos, pos - startPos), startLine, startColumn);
            } else {
                token.type = TokenType::TOKEN_ERROR;
                token.typeName = "ERROR";
                token.lexeme = sourceCode.substr(startPos, pos - startPos);
                token.line = startLine;
                token.column = startColumn;
            }
            tokens.push_back(token);
            continue;
        }

        string bestMatch = "";
        string bestTokenType = "";
        size_t bestLength = 0;
        
        // Tenta todas as regras para encontrar o match mais longo.
        // Em empate de tamanho, a regra adicionada primeiro vence.
        for (const auto& rule : rules) {
            const string& tokenType = rule.first;
            const auto& automaton = rule.second;
            
            if (!automaton) continue;
            
            // Tenta substrings crescentes desta posição
            for (size_t len = 1; len <= sourceCode.length() - pos; ++len) {
                string candidate = sourceCode.substr(pos, len);
                
                // Se o autômato aceita e é mais longo que antes, guarda
                if (automaton->accepts(candidate)) {
                    if (len > bestLength) {
                        bestMatch = candidate;
                        bestTokenType = tokenType;
                        bestLength = len;
                    }
                }
            }
        }
        
        if (bestLength > 0) {
            Token token;
            token.typeName = bestTokenType;
            token.lexeme = bestMatch;
            token.line = line;
            token.column = column;
            
            // Mapeia tipo de nome para TokenType
            if (bestTokenType.find("WHITESPACE") != string::npos) {
                token.type = TokenType::TOKEN_WHITESPACE;
            } else if (bestTokenType.find("NUMBER") != string::npos || 
                      bestTokenType.find("INTEGER") != string::npos ||
                      bestTokenType.find("FLOAT") != string::npos) {
                token.type = TokenType::TOKEN_NUMBER;
            } else if (bestTokenType.find("STRING") != string::npos) {
                token.type = TokenType::TOKEN_STRING;
            } else if (bestTokenType.find("LPAREN") != string::npos) {
                token.type = TokenType::TOKEN_LPAREN;
            } else if (bestTokenType.find("RPAREN") != string::npos) {
                token.type = TokenType::TOKEN_RPAREN;
            } else if (bestTokenType.find("SYMBOL") != string::npos ||
                      bestTokenType.find("IDENTIFIER") != string::npos) {
                token.type = TokenType::TOKEN_IDENTIFIER;
            } else {
                token.type = TokenType::TOKEN_DYNAMIC;
            }
            
            bool ignoreToken = bestTokenType.find("WHITESPACE") != string::npos ||
                              bestTokenType.find("COMMENT") != string::npos ||
                              bestTokenType.find("NEWLINE") != string::npos ||
                              bestTokenType.find("LANG_DIRECTIVE") != string::npos;
            if (!ignoreToken) {
                tokens.push_back(token);
            }
            
            // Atualiza posição e rastreia linhas/colunas
            for (char c : bestMatch) {
                if (c == '\n') {
                    line++;
                    column = 1;
                } else {
                    column++;
                }
            }
            
            pos += bestLength;
        } else {
            // Nenhuma regra casou: erro
            Token errorToken;
            errorToken.type = TokenType::TOKEN_ERROR;
            errorToken.typeName = "ERROR";
            errorToken.lexeme = sourceCode.substr(pos, 1);
            errorToken.line = line;
            errorToken.column = column;
            
            tokens.push_back(errorToken);
            
            if (sourceCode[pos] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
        }
    }
    
    // Adiciona EOF
    Token eofToken;
    eofToken.type = TokenType::TOKEN_EOF;
    eofToken.typeName = "EOF";
    eofToken.lexeme = "";
    eofToken.line = line;
    eofToken.column = column;
    tokens.push_back(eofToken);
    
    return tokens;
}

int Scanner::getRuleCount() const {
    return static_cast<int>(rules.size());
}
