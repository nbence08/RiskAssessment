#ifndef OPERATION_H
#define OPERATION_H

#include "model.hpp"

class Connection;
class LPriv;
class PAccess;
class Service;

class Operation : public Model {
private:
    std::vector<std::weak_ptr<Connection>> containsConnections;
    std::vector<std::weak_ptr<LPriv>> containedLPrivs;
    std::vector<std::weak_ptr<PAccess>> pAccesses;

    std::vector<std::weak_ptr<LPriv>> requiredLPrivs;
    std::shared_ptr<LPriv> lPriv;
    Service* service;

    class OperationGrantPAccessVisitor : public ModelVisitor {
    public:
        OperationGrantPAccessVisitor(Model* model): ModelVisitor(model) {}

        void visits(PhysicalZone& pzone) override;
        void visits(Device& device) override;
    };

    class OperationGrantLPrivVisitor : public ModelVisitor {
    public:
        OperationGrantLPrivVisitor(Model* model): ModelVisitor(model) {}

        void visits(Operation& operation) override;
        void visits(Service& service) override;
    };

    class OperationGrantConnectionVisitor : public ModelVisitor {
    public:
        OperationGrantConnectionVisitor(Model* model): ModelVisitor(model) {}

        void visits(Service& service) override;
        void visits(Operation& operation) override;
    };

    class OperationCreateAssociationToVisitor : public ModelVisitor {
    public:
        OperationCreateAssociationToVisitor(Model* model): ModelVisitor(model) {}

        void visits(Service& service) override;
    };

    static int idCount;
public:

    explicit Operation(QObject *parent = nullptr);
    Operation(QString name, QObject* parent = nullptr);
    void loadConnections(QJsonObject, std::unordered_map<QString, Serializable*>) override;

    QJsonObject serialize() const override;

    std::weak_ptr<LPriv> getLPriv(){
        return std::weak_ptr<LPriv>(lPriv);
    }

    virtual std::unique_ptr<ModelVisitor> getGrantPAccessVisitor() override {
        return std::make_unique<OperationGrantPAccessVisitor>(this);
    }
    virtual std::unique_ptr<ModelVisitor> getGrantLPrivVisitor() override {
        return std::make_unique<OperationGrantLPrivVisitor>(this);
    }
    virtual std::unique_ptr<ModelVisitor> getGrantConnectionVisitor() override {
        return std::make_unique<OperationGrantConnectionVisitor>(this);
    }
    virtual std::unique_ptr<ModelVisitor> getCreateAssociationToVisitor() override {
        return std::make_unique<OperationCreateAssociationToVisitor>(this);
    }

    void visitedBy(ModelVisitor& visitor) override {
        visitor.visits(*this);
    }

    void removeConnectionTo(QString serviceName);
    void removePAccessTo(QString paccName);
    void removeLPrivTo(QString lprivName);
    void removeServiceTo(QString name);
};

#endif // OPERATION_H
