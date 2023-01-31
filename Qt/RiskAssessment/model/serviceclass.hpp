#ifndef SERVICECLASS_HPP
#define SERVICECLASS_HPP

#include <vector>

#include "model.hpp"

class ServiceClass : public Model
{
private:
    std::vector<Service*> services;

    class AddServiceToServiceClassVisitor : public ModelVisitor{
    private:
        ServiceClass& serviceClass;
    public:
        explicit AddServiceToServiceClassVisitor(ServiceClass& service);

        void visits(Service& service);
    };

public:
    explicit ServiceClass(QObject* parent = nullptr);
    ServiceClass(QString name, QObject* parent = nullptr);


    // comparator is a unary operation which has a callable `bool operator()(Service*)`
    template <typename COMPARATOR>
    void removeService(COMPARATOR comparator);

    void removeServiceByName(QString name);
    void removeServiceByPtr(Service* service);
    void addService(Service& service);

    QJsonObject serialize() const override;
    void loadConnections(QJsonObject jsonObj, std::unordered_map<QString, Serializable*> addressTable) override;

    std::unique_ptr<ModelVisitor> getCreateAssociationToVisitor() override {
        return std::make_unique<AddServiceToServiceClassVisitor>(*this);
    }

    void visitedBy(ModelVisitor& visitor){
        visitor.visits(*this);
    }
};

template <typename COMPARATOR>
void ServiceClass::removeService(COMPARATOR comparator){
    auto serviceIt = std::find_if(services.begin(), services.end(), comparator);

    if(serviceIt != services.end()){
        emit associationDeleted(this, *serviceIt);
        services.erase(serviceIt);
    }
}

#endif // SERVICECLASS_HPP
