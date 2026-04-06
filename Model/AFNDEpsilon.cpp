#include "AFNDEpsilon.hpp"
#include <algorithm>
#include <queue>

using namespace std;

set<int> AFNDEpsilon::epsilonClosure(const set<int>& states) const {
    // TODO: Implementar
    return states;
}

set<int> AFNDEpsilon::transitionOn(
    const set<int>& states, 
    char symbol
) const {
    // TODO: Implementar
    return {};
}

bool AFNDEpsilon::containsAcceptState(const set<int>& states) const {
    // TODO: Implementar
    return false;
}

bool AFNDEpsilon::accepts(const string& input) const {
    // TODO: Implementar
    return false;
}