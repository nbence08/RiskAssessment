#ifndef LPRIV_H
#define LPRIV_H

#include "model.hpp"

class Operation;

//Logical Privilege
class LPriv : public Model {
private:
    Operation& operation;
protected:

    /*virtual QString getNextID() override {
        return QString::number(idCounter++);
    }*/
public:
    LPriv(Operation& op, QObject* parent = nullptr);

    Operation& getOperation();

    void visitedBy(ModelVisitor& visitor) override {
        visitor.visits(*this);
    }
};

#endif // LPRIV_H
