#include "paccess.hpp"
#include "physicalzone.hpp"

PAccess::PAccess(PhysicalZone& pzone, QObject* parent):Model("PA", parent), pZone(pzone){}

PhysicalZone& PAccess::getPZone() { return pZone; }
