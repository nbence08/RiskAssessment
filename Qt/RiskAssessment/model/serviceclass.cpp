#include "serviceclass.hpp"

#include <algorithm>

#include "model/service.hpp"

ServiceClass::ServiceClass(QObject* parent): Model{"SC", parent}{

}

ServiceClass::ServiceClass(QString name, QObject* parent):Model{name, "SC", parent}{

}

void ServiceClass::removeServiceByName(QString name){
    this->removeService([&name](Service* serv){return serv->objectName().compare(name) == 0;});
}

void ServiceClass::removeServiceByPtr(Service* service){
    this->removeService([service](Service* other){ return service == other; });
}

void ServiceClass::addService(Service& service){
    services.push_back(&service);
    emit modelAssociatedToModel({this, &service, "HasService"});
}

QJsonObject ServiceClass::serialize() const{
    QJsonObject jsonObj;
    jsonObj[NAME] = objectName();
    jsonObj[TYPE] = SERVICECLASS_TYPENAME;

    QJsonArray servicesArr;
    for(auto service:services){
        servicesArr.append(service->objectName());
    }

    jsonObj[SERVICECLASS_CONTAINS_SERVICE];

    return jsonObj;
}

void ServiceClass::loadConnections(QJsonObject jsonObj, std::unordered_map<QString, Serializable*> addressTable){
    QJsonArray servicesJsonArr = jsonObj[SERVICECLASS_CONTAINS_SERVICE].toArray();

    for(auto name:servicesJsonArr){
        if(addressTable.contains(name.toString())){
            Service* service = static_cast<Service*>(addressTable.at(name.toString()));
            addService(*service);
        }
    }
}

ServiceClass::AddServiceToServiceClassVisitor::AddServiceToServiceClassVisitor(ServiceClass& serviceClass):ModelVisitor(&serviceClass), serviceClass(serviceClass) {}

void ServiceClass::AddServiceToServiceClassVisitor::visits(Service& service){
    serviceClass.addService(service);
}
