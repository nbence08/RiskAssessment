#include "device.hpp"
#include "logicalzone.hpp"
#include "physicalzone.hpp"
#include "service.hpp"
#include "role.hpp"

Device::Device(QObject *parent):Model{"DV", parent}{
    lZone = nullptr;
    pZone = nullptr;
}

Device::Device(QString name, QObject* parent): Model(name, "DV", parent) {
    lZone = nullptr;
    pZone = nullptr;
}

QJsonObject Device::serialize() const {
    QJsonObject jsonDev;
    jsonDev["name"] = objectName();
    jsonDev["type"] = "Device";

    if(pZone != nullptr){
        jsonDev[PZONE_CONTAINS_DEVICE] = pZone->objectName();
    }
    if(lZone != nullptr){
        jsonDev[LZONE_CONTAINS_DEVICE] = lZone->objectName();
    }

    QJsonArray jsonServices;
    for(const auto service:services){
        jsonServices.append(service->objectName());
    }

    jsonDev[DEVICE_HAS_SERVICE] = jsonServices;

    return jsonDev;
}

void Device::loadConnections(QJsonObject obj, std::unordered_map<QString, Serializable*> addressTable){

    auto physicalZoneName = obj[PZONE_CONTAINS_DEVICE].toString();
    if(addressTable.contains(physicalZoneName)){
        PhysicalZone* pz = static_cast<PhysicalZone*>(addressTable.at(physicalZoneName));
        this->requestConnectToZoneVisit(*pz);
    }

    auto logicalZoneName = obj[LZONE_CONTAINS_DEVICE].toString();
    if(addressTable.contains(logicalZoneName)){
        LogicalZone* lz = static_cast<LogicalZone*>(addressTable.at(logicalZoneName));
        this->requestConnectToZoneVisit(*lz);
    }

    auto serviceNames = obj[DEVICE_HAS_SERVICE].toArray();
    for(auto name:serviceNames){
        Service* service= static_cast<Service*>(addressTable.at(name.toString()));
        this->requestCreateAssociationToModel(*service);
    }
}

void Device::removeServiceByName(QString name){
    auto servIt = std::find_if(services.begin(), services.end(), [name](Service* service){
            return service->objectName().compare(name) == 0;
    });
    if(servIt != services.end()){
        emit associationDeleted(this, *servIt);
        services.erase(servIt);
    }
}

void Device::disconnectZoneByName(QString name){
    if(lZone != nullptr){
        if(lZone->objectName().compare(name) == 0){
            emit itemDisconnectedFromZone(this, lZone);
            lZone = nullptr;
        }
    }
    if(pZone != nullptr){
        if(pZone->objectName().compare(name) == 0){
            emit itemDisconnectedFromZone(this, pZone);
            pZone = nullptr;
        }
    }
}

Device::DeviceConnectWithZoneVisitor::DeviceConnectWithZoneVisitor(Model* device):ModelVisitor(device){}

void Device::DeviceConnectWithZoneVisitor::visits(LogicalZone& lZone){
    Device* device = static_cast<Device*>(model);
    if(device->lZone == &lZone){
        return;
    }
    if(device->lZone != nullptr){
        emit model->itemDisconnectedFromZone(model, device->lZone);
    }

    device->lZone = &lZone;
    emit model->itemConnectedToZone(model, &lZone);
}

void Device::DeviceConnectWithZoneVisitor::visits(PhysicalZone& pZone){
    Device* device = static_cast<Device*>(model);
    if(device->pZone == &pZone){
        return;
    }
    if(device->pZone != nullptr){
        emit model->itemDisconnectedFromZone(model, device->pZone);
    }

    device->pZone = &pZone;
    emit model->itemConnectedToZone(model, &pZone);
}

Device::DeviceAddServiceVisitor::DeviceAddServiceVisitor(Device& device): ModelVisitor(&device), device(device) {}

void Device::DeviceAddServiceVisitor::visits(Service& service){
    device.services.insert(&service);
    if(service.getDevice() != nullptr){
        service.getDevice()->removeServiceByName(service.objectName());
    }
    service.setDevice(static_cast<Device*>(model));
    emit model->modelAssociatedToModel({model, &service, "HasService"});
}
