#include "employee.hpp"

#include "operation.hpp"
#include "physicalzone.hpp"
#include "logicalzone.hpp"
#include "service.hpp"

#include "connection.hpp"
#include "lpriv.hpp"
#include "paccess.hpp"
#include "role.hpp"

Employee::Employee(QObject* parent): Model("EM", parent){

}

Employee::Employee(QString name, QObject* parent): Model(name, "EM", parent) {  }

QJsonObject Employee::serialize() const {
    QJsonObject jsonObj;
    jsonObj["name"] = objectName();
    jsonObj["type"] = "Employee";

    QJsonArray jsonConnections;
    QJsonArray jsonLPrivs;
    QJsonArray jsonPAccesses;
    QJsonArray jsonRoles;

    for(const auto& conn:connections){
        jsonConnections.push_back(conn.lock()->getService().objectName());
    }

    for(const auto& lpriv:lPrivs){
        jsonLPrivs.push_back(lpriv.lock().get()->getOperation().objectName());
    }

    for(const auto& pacc:pAccesses){
        jsonPAccesses.push_back(pacc.lock()->getPZone().objectName());
    }

    for(const auto& role:roles){
        jsonRoles.push_back(role->objectName());
    }

    jsonObj[HAS_CONNECTION] = jsonConnections;
    jsonObj[HAS_LPRIV] = jsonLPrivs;
    jsonObj[HAS_PACCESS] = jsonPAccesses;
    jsonObj[HAS_ROLE] = jsonRoles;

    return jsonObj;
}

void Employee::loadConnections(QJsonObject jsonObj, std::unordered_map<QString, Serializable*> addressTable){
    auto connectionNames = jsonObj[HAS_CONNECTION].toArray();
    auto lprivNames = jsonObj[HAS_LPRIV].toArray();
    auto paccNames = jsonObj[HAS_PACCESS].toArray();
    auto roleNames = jsonObj[HAS_ROLE].toArray();

    for(auto conn:connectionNames){
        Service* service = static_cast<Service*>(addressTable.at(conn.toString()));
        this->requestGrantConnectionVisit(*service);
    }

    for(auto lpriv:lprivNames){
        Operation* op = static_cast<Operation*>(addressTable.at(lpriv.toString()));
        this->requestGrantLPrivVisit(*op);
    }

    for(auto pacc:paccNames){
        PhysicalZone* pz = static_cast<PhysicalZone*>(addressTable.at(pacc.toString()));
        this->requestGrantPAccessVisit(*pz);
    }

    for(auto roleName:roleNames){
        Role* role = static_cast<Role*>(addressTable.at(roleName.toString()));
        this->requestGrantLPrivVisit(*role);
    }
}

void Employee::removeLPrivForOperation(QString opName){
    auto privIt = lPrivs.begin();
    for(;privIt != lPrivs.end(); ++privIt){
        auto& privOp = privIt->lock()->getOperation();
        auto privOpName = privOp.objectName();

        if(privOpName.compare(opName) == 0){
            lPrivs.erase(privIt);
            emit this->lprivDeleted(this, &privOp);
            return;
        }
    }
}

void Employee::removeLPrivLike(QString name){
    auto roleIt = std::find_if(roles.begin(), roles.end(), [&name=name](Role* role){ return role->objectName().compare(name) == 0;});
    if(roleIt != roles.end()){
        roles.erase(roleIt);
        emit lprivDeleted(this, *roleIt);
        return;
    }

    auto privIt = std::find_if(lPrivs.begin(), lPrivs.end(), [&name=name](std::weak_ptr<LPriv> priv){ return priv.lock()->getOperation().objectName().compare(name) == 0;});
    if(privIt != lPrivs.end()){
        lPrivs.erase(privIt);
        emit this->lprivDeleted(this, &(privIt->lock()->getOperation()));
        return;
    }
}

void Employee::removeConnectionTo(QString serviceName) {
    auto connIt = std::find_if(connections.begin(), connections.end(), [name=serviceName](std::weak_ptr<Connection> conn){
        return conn.lock()->getService().objectName().compare(name) == 0;
    });
    if(connIt != connections.end()){
        connections.erase(connIt);
        emit this->connectionDeleted(this, &(connIt->lock()->getService()));
    }
}

void Employee::removePAccessTo(QString paccName) {
    auto paccessIt = std::find_if(pAccesses.begin(), pAccesses.end(), [name=paccName](std::weak_ptr<PAccess> pacc){
        return pacc.lock()->getPZone().objectName().compare(name) == 0;
    });
    if(paccessIt != pAccesses.end()){
        pAccesses.erase(paccessIt);
        emit this->pAccessDeleted(this, &(paccessIt->lock()->getPZone()));
    }
}

void Employee::EmployeeGrantPAccessVisitor::visits(PhysicalZone& pzone) {
    auto paccess = pzone.getPAccess();
    ((Employee*)this->model)->pAccesses.push_back(paccess);
    emit model->pAccessGranted(model, &pzone);
}

//TODO Lpriv is granted for operation, I think editor could have the option to give lprivs for all operations on a device/service
void Employee::EmployeeGrantPAccessVisitor::visits(Device& /*device*/) {
    throw std::logic_error("further deliberation required");
}

void Employee::EmployeeGrantLPrivVisitor::visits(Role& role){
    auto emp = static_cast<Employee*>(model);
    emp->roles.push_back(&role);

    emit emp->lPrivGranted(emp, &role);
}

void Employee::EmployeeGrantLPrivVisitor::visits(Operation& operation)
{
    auto lPriv = operation.getLPriv();
    ((Employee*)this->model)->lPrivs.push_back(lPriv);
    emit model->lPrivGranted(model, &operation);
}

void Employee::EmployeeGrantLPrivVisitor::visits(Service& /*service*/)
{
    throw std::logic_error("further deliberation required");
}

void Employee::EmployeeGrantConnectionVisitor::visits(Service& service)
{
    auto conn = service.getConnection();
    ((Employee*)this->model)->connections.push_back(conn);
    emit model->connectionAddedFromTo(model, &service);
}

void Employee::EmployeeGrantConnectionVisitor::visits(Operation& /*operation*/)
{
    throw std::logic_error("further deliberation required");
}
