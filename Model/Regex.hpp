#ifndef REGEX_HPP
#define REGEX_HPP

#include <string>
#include <vector>

/**
 * Regex: Representa uma expressão regular estruturada
 * A expressão é armazenada em notação polonesa reversa (RPN)
 * para facilitar a construção de Thompson
 */
class Regex {
private:
    std::string originalExpression;  // Expressão original
    std::vector<std::string> rpn;    // Notação polonesa reversa
    std::string tokenType;           // Tipo de token que essa regex reconhece
    
public:
    explicit Regex(const std::string& expression = "");
    
    /**
     * Define a expressão regular
     */
    void setExpression(const std::string& expression);
    
    /**
     * Obtém a expressão original
     */
    std::string getExpression() const;
    
    /**
     * Converte para notação polonesa reversa
     */
    void toRPN();
    
    /**
     * Obtém a representação em RPN
     */
    const std::vector<std::string>& getRPN() const;
    
    /**
     * Define o tipo de token
     */
    void setTokenType(const std::string& type);
    
    /**
     * Obtém o tipo de token
     */
    std::string getTokenType() const;
};

#endif // REGEX_HPP