#include "AutomatonBase.hpp"
#include "AutomatonFactory.hpp"

// AutomatonBase não precisa de implementação (tudo é inline no header)

// Implementação de AutomatonFactory::createState()
State AutomatonFactory::createState() {
    return State(stateCounter++);
}
