#ifndef REGEX_HPP
#define REGEX_HPP

#include <string>
#include <vector>

using namespace std;

/**
 * Regex: Representa uma expressão regular estruturada
 * A expressão é armazenada em notação polonesa reversa (RPN)
 * para facilitar a construção de Thompson
 */
class Regex {
private:
    string originalExpression;  // Expressão original
    vector<string> rpn;    // Notação polonesa reversa
    string tokenType;           // Tipo de token que essa regex reconhece
    
public:
    explicit Regex(const string& expression = "");
    
    /**
     * Define a expressão regular
     */
    void setExpression(const string& expression);
    
    /**
     * Obtém a expressão original
     */
    string getExpression() const;
    
    /**
     * Converte para notação polonesa reversa
     */
    void toRPN();
    
    /**
     * Obtém a representação em RPN
     */
    const vector<string>& getRPN() const;
    
    /**
     * Define o tipo de token
     */
    void setTokenType(const string& type);
    
    /**
     * Obtém o tipo de token
     */
    string getTokenType() const;
};

#endif // REGEX_HPP