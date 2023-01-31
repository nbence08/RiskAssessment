#include "lpriv.hpp"
#include "operation.hpp"

LPriv::LPriv(Operation& op, QObject* parent):Model("LP", parent), operation(op) {}

Operation& LPriv::getOperation(){ return operation; }
