#ifndef SERVICE_H
#define SERVICE_H

#include <vector>

#include "model.hpp"

class Operation;
class Connection;

class Service : public Model {
private:
    std::vector<Operation*> operations;
    std::shared_ptr<Connection> connection;

    Device* device;

    class ServiceCreateAssociationToVisitor : public ModelVisitor {
    public:
        ServiceCreateAssociationToVisitor(Model* model): ModelVisitor(model) {}

        void visits(Operation& operation) override;
    };
protected:
    /*virtual QString getNextID() override {
        return QString::number(idCounter++);
    }*/
public:
    Service(QObject* parent = nullptr);
    Service(QString name, QObject* parent = nullptr);
    std::weak_ptr<Connection> getConnection(){
        return std::weak_ptr<Connection>(connection);
    }

    Device* getDevice(){ return device; }
    void setDevice(Device* device){ this->device = device; }

    QJsonObject serialize() const override;
    void loadConnections(QJsonObject jsonObj, std::unordered_map<QString, Serializable*> addressTable) override;

    virtual std::unique_ptr<ModelVisitor> getCreateAssociationToVisitor() override {
        return std::make_unique<ServiceCreateAssociationToVisitor>(this);
    }

    void visitedBy(ModelVisitor& visitor) override {
        visitor.visits(*this);
    }

    //signal emitter methods
    void removeOperationByName(QString name);
};

#endif // SERVICE_H
