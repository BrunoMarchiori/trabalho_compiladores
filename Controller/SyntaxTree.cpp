#include "SyntaxTree.hpp"
#include <sstream>
#include <algorithm>

std::string SyntaxTree::toString(int depth) const {
    std::string indent(depth * 2, ' ');
    std::ostringstream oss;
    
    oss << indent << "[";
    
    switch (type) {
        case NodeType::PROGRAM:           oss << "PROGRAM"; break;
        case NodeType::LITERAL_NUM:       oss << "NUM:" << value; break;
        case NodeType::LITERAL_BOOL:      oss << "BOOL:" << value; break;
        case NodeType::LITERAL_STRING:    oss << "STRING:" << value; break;
        case NodeType::SYMBOL:            oss << "SYM:" << value; break;
        case NodeType::DEFINE:            oss << "DEFINE"; break;
        case NodeType::LAMBDA:            oss << "LAMBDA"; break;
        case NodeType::IF:                oss << "IF"; break;
        case NodeType::COND:              oss << "COND"; break;
        case NodeType::LET:               oss << "LET"; break;
        case NodeType::FUNCTION_CALL:     oss << "CALL"; break;
        case NodeType::BINDING_LIST:      oss << "BINDINGS"; break;
        case NodeType::COND_CLAUSE:       oss << "CLAUSE"; break;
        case NodeType::QUOTE:             oss << "QUOTE"; break;
        case NodeType::LIST:              oss << "LIST"; break;
    }
    
    oss << "]";
    
    if (!children.empty()) {
        oss << "\n";
        for (const auto& child : children) {
            oss << child->toString(depth + 1) << "\n";
        }
        oss << indent;
    }
    
    return oss.str();
}

int SyntaxTree::getNodeCount() const {
    int count = 1;
    for (const auto& child : children) {
        count += child->getNodeCount();
    }
    return count;
}

int SyntaxTree::getDepth() const {
    if (children.empty()) return 1;
    int maxChildDepth = 0;
    for (const auto& child : children) {
        maxChildDepth = std::max(maxChildDepth, child->getDepth());
    }
    return 1 + maxChildDepth;
}
