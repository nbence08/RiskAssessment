#ifndef PACCESS_H
#define PACCESS_H

#include "model.hpp"

class PhysicalZone;

//Physical Access
class PAccess : public Model {
private:
    PhysicalZone& pZone;
public:
    PAccess(PhysicalZone& pzone, QObject* parent = nullptr);

    PhysicalZone& getPZone();

    void visitedBy(ModelVisitor& visitor) override {
        visitor.visits(*this);
    }
};

#endif // PACCESS_H
