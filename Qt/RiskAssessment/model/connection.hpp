#ifndef CONNECTION_H
#define CONNECTION_H

#include "model.hpp"

class Service;
class Connection : public Model {
private:
    Service& service;
public:
    Connection(Service& service, QObject* parent = nullptr);

    void visitedBy(ModelVisitor& visitor) override {
        visitor.visits(*this);
    }

    Service& getService(){ return service; }
};

#endif // CONNECTION_H
