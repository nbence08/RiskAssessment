#include "service.hpp"

#include "operation.hpp"
#include "connection.hpp"

Service::Service(QObject* parent): Model("SV", parent){
    connection = std::make_shared<Connection>(*this);
    device = nullptr;
}

Service::Service(QString name, QObject* parent): Model(name, "SV", parent) {
    connection = std::make_shared<Connection>(*this);
    device = nullptr;
}

QJsonObject Service::serialize() const {
    QJsonObject jsonObj;
    jsonObj["name"] = objectName();
    jsonObj["type"] = "Service";

    QJsonArray jsonOps;
    for(auto op:operations){
        jsonOps.push_back(op->objectName());
    }

    jsonObj[SERVICE_CONTAINS_OPERATION] = jsonOps;

    return jsonObj;
}

void Service::loadConnections(QJsonObject jsonObj, std::unordered_map<QString, Serializable*> addressTable){
    auto jsonOps = jsonObj[SERVICE_CONTAINS_OPERATION].toArray();

    for(auto name:jsonOps){
        Operation* op = static_cast<Operation*>(addressTable.at(name.toString()));
        requestCreateAssociationToModel(*op);
    }
}

void Service::removeOperationByName(QString name){
    auto opIt = operations.begin();
    for(;opIt != operations.end(); ++opIt){
        if((*opIt)->objectName().compare(name) == 0){
            emit associationDeleted(this, *opIt);
            operations.erase(opIt);
            return;
        }
    }
}

void Service::ServiceCreateAssociationToVisitor::visits(Operation& operation) {
    ((Service*) model)->operations.push_back(&operation);
    emit model->modelAssociatedToModel({model, &operation, "HasOperation"});
}
