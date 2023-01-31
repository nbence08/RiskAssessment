#ifndef PATH_H
#define PATH_H

#include <vector>

#include "model.hpp"

class PhysicalZone;

class Path : public Model{
private:
    std::vector<PhysicalZone*> pZones;
protected:
    /*virtual QString getNextID() override {
        return QString::number(idCounter++);
    }*/
public:
    Path(QObject *parent = nullptr);

    void visitedBy(ModelVisitor& visitor) override {
        visitor.visits(*this);
    }
};

#endif // PATH_H
