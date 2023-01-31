#include "physicalzone.hpp"
#include "device.hpp"
#include "paccess.hpp"

PhysicalZone::PhysicalZone(QObject *parent): Model{"PZ", parent} {
    pAccess = std::make_shared<PAccess>(*this);
}

PhysicalZone::PhysicalZone(QString name, QObject* parent): Model(name, "PZ", parent) {
    pAccess = std::make_shared<PAccess>(*this);
}

std::weak_ptr<PAccess> PhysicalZone::getPAccess() {
    return std::weak_ptr<PAccess>(pAccess);
}

QJsonObject PhysicalZone::serialize() const {
    QJsonObject jsonPZone;
    jsonPZone["name"] = objectName();
    jsonPZone["type"] = "PhysicalZone";

    QJsonArray jsonNeighbors;
    for(auto zone:neighbors){
        jsonNeighbors.append(zone->objectName());
    }
    jsonPZone[PZONE_HAS_NEIGHBOR] = jsonNeighbors;

    return jsonPZone;
}

int PhysicalZone::idCount = 0;

void PhysicalZone::loadConnections(QJsonObject jsonObj, std::unordered_map<QString, Serializable *> addressTable){
    auto neighborsArr = jsonObj[PZONE_HAS_NEIGHBOR].toArray();

    for(auto item:neighborsArr){
        if(addressTable.contains(item.toString())){
            Model* otherPzone = static_cast<Model*>(addressTable.at(item.toString()));
            requestConnectToZoneVisit(*otherPzone);
        }
    }
}
