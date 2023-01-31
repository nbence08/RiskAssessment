#include "operation.hpp"

#include "service.hpp"
#include "physicalzone.hpp"
#include "model/lpriv.hpp"
#include "connection.hpp"
#include "paccess.hpp"
#include "lpriv.hpp"

Operation::Operation(QObject *parent)
    : Model{"OP", parent}
{
    lPriv = std::make_shared<LPriv>(*this);
    service = nullptr;
}

Operation::Operation(QString name, QObject* parent): Model(name, "OP", parent) {
    lPriv = std::make_shared<LPriv>(*this);
    service = nullptr;
}

int Operation::idCount = 0;

QJsonObject Operation::serialize() const {
    QJsonObject jsonObj;
    jsonObj["name"] = objectName();
    jsonObj["type"] = "Operation";

    QJsonArray jsonConnections;
    QJsonArray jsonPAccesses;
    QJsonArray jsonLPrivs;

    for(const auto& conn:containsConnections){
        jsonConnections.push_back(conn.lock()->getService().objectName());
    }

    for(const auto& pacc:pAccesses){
        jsonPAccesses.push_back(pacc.lock()->getPZone().objectName());
    }

    for(const auto& lp:containedLPrivs){
        jsonLPrivs.append(lp.lock()->getOperation().objectName());
    }

    jsonObj[OPERATION_CONTAINS_CONN] = jsonConnections;
    jsonObj[OPERATION_CONTAINS_PACC] = jsonPAccesses;
    jsonObj[OPERATION_CONTAINS_LPRIV] = jsonLPrivs;

    return jsonObj;
}

void Operation::loadConnections(QJsonObject jsonObj, std::unordered_map<QString, Serializable *> addressTable) {
    auto jsonConnections = jsonObj[OPERATION_CONTAINS_CONN].toArray();
    auto jsonPAccesses = jsonObj[OPERATION_CONTAINS_PACC].toArray();
    auto jsonContainedLPrivs = jsonObj[OPERATION_CONTAINS_LPRIV].toArray();

    for(auto name:jsonConnections){
        Service* service = static_cast<Service*>(addressTable.at(name.toString()));
        requestGrantConnectionVisit(*service);
    }

    for(auto name:jsonPAccesses){
        PhysicalZone* pz = static_cast<PhysicalZone*>(addressTable.at(name.toString()));
        requestGrantPAccessVisit(*pz);
    }

    for(auto name:jsonContainedLPrivs){
        Operation* op = static_cast<Operation*>(addressTable.at(name.toString()));
        requestGrantLPrivVisit(*op);
    }
}

void Operation::removeConnectionTo(QString serviceName) {
    auto connIt = std::find_if(containsConnections.begin(), containsConnections.end(), [name=serviceName](std::weak_ptr<Connection> conn){
        return conn.lock()->getService().objectName().compare(name) == 0;
    });
    if(connIt != containsConnections.end()){
        emit this->connectionDeleted(this, &(connIt->lock()->getService()));
        containsConnections.erase(connIt);
    }
}

void Operation::removePAccessTo(QString paccName) {
    auto paccessIt = std::find_if(pAccesses.begin(), pAccesses.end(), [name=paccName](std::weak_ptr<PAccess> pacc){
        return pacc.lock()->getPZone().objectName().compare(name) == 0;
    });
    if(paccessIt != pAccesses.end()){
        emit this->pAccessDeleted(this, &(paccessIt->lock()->getPZone()));
        pAccesses.erase(paccessIt);
    }
}

void Operation::removeLPrivTo(QString lprivName){
    auto lprivIt = std::find_if(containedLPrivs.begin(), containedLPrivs.end(), [name=lprivName](std::weak_ptr<LPriv> lpriv){
        return lpriv.lock()->getOperation().objectName().compare(name) == 0;
    });
    if(lprivIt != containedLPrivs.end()){
        emit this->lprivDeleted(this, &(lprivIt->lock()->getOperation()));
        containedLPrivs.erase(lprivIt);
    }
}

void Operation::removeServiceTo(QString name){
    if(this->service->objectName().compare(name) == 0){
        emit associationDeleted(this, service);
        this->service = nullptr;
    }
}

void Operation::OperationGrantPAccessVisitor::visits(PhysicalZone& pzone) {
    auto paccess = pzone.getPAccess();
    ((Operation*)this->model)->pAccesses.push_back(paccess);
    emit model->pAccessGranted(model, &pzone);
}

void Operation::OperationGrantPAccessVisitor::visits(Device& /*device*/) {
    throw std::logic_error("further deliberation required");
}

void Operation::OperationGrantLPrivVisitor::visits(Operation& /*operation*/)
{
    throw std::logic_error("further deliberation required");
}

void Operation::OperationGrantLPrivVisitor::visits(Service& /*service*/)
{
    throw std::logic_error("further deliberation required");
}

void Operation::OperationGrantConnectionVisitor::visits(Service& service){
    auto conn = service.getConnection();
    ((Operation*)this->model)->containsConnections.push_back(conn);
    emit model->connectionAddedFromTo(model, &service);
}

void Operation::OperationGrantConnectionVisitor::visits(Operation& /*operation*/)
{
    throw std::logic_error("further deliberation required");
}

void Operation::OperationCreateAssociationToVisitor::visits(Service& service) {
    ((Operation*) model)->service = &service;
    emit model->modelAssociatedToModel({model, &service, "HasConnectionTo"});
}
