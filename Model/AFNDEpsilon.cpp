#include "AFNDEpsilon.hpp"
#include <algorithm>
#include <queue>

std::set<int> AFNDEpsilon::epsilonClosure(const std::set<int>& states) const {
    // TODO: Implementar
    return states;
}

std::set<int> AFNDEpsilon::transitionOn(
    const std::set<int>& states, 
    char symbol
) const {
    // TODO: Implementar
    return {};
}

bool AFNDEpsilon::containsAcceptState(const std::set<int>& states) const {
    // TODO: Implementar
    return false;
}

bool AFNDEpsilon::accepts(const std::string& input) const {
    // TODO: Implementar
    return false;
}