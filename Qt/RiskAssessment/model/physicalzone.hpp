#ifndef PHYSICALZONE_H
#define PHYSICALZONE_H

#include <vector>
#include <set>
#include <algorithm>

#include "model.hpp"

class Device;

class PhysicalZone : public Model {
private:
    std::shared_ptr<PAccess> pAccess;

    std::vector<PhysicalZone*> neighbors;

    class PZoneConnectToPZoneVisitor : public ModelVisitor {
    private:
        PhysicalZone& pzone;
    public:
        PZoneConnectToPZoneVisitor(PhysicalZone& pzone): ModelVisitor(&pzone), pzone(pzone) {}

        void visits(PhysicalZone& otherZone) override {
            pzone.addNeighbor(otherZone);
        }
    };

protected:
    void addNeighborToArray(PhysicalZone* zone){
        neighbors.push_back(zone);
    }

    void removeNeighborFromArray(PhysicalZone* zone){
        std::remove(neighbors.begin(), neighbors.end(), zone);
    }
public:
    explicit PhysicalZone(QObject* parent = nullptr);
    PhysicalZone(QString name, QObject* parent = nullptr);
    static int idCount;

    std::weak_ptr<PAccess> getPAccess();

    std::unique_ptr<ModelVisitor> getConnectToZoneVisitor() override {
        return std::make_unique<PZoneConnectToPZoneVisitor>(*this);
    }

    void addNeighbor(PhysicalZone& zone){
        if(std::find(neighbors.begin(), neighbors.end(), &zone) != neighbors.end()){
            return;
        }
        neighbors.push_back(&zone);
        zone.addNeighborToArray(this);
        emit Model::itemConnectedToZone(this, &zone);
    }

    void removeNeighbor(PhysicalZone& zone){
        auto neighborIt = std::find(neighbors.begin(), neighbors.end(), &zone);
        if(neighborIt != neighbors.end()){
            zone.removeNeighborFromArray(this);
            emit Model::itemDisconnectedFromZone(this, &zone);
        }
    }

    void removeNeighborByName(QString name){
        auto zoneIt = std::find_if(neighbors.begin(), neighbors.end(), [name](PhysicalZone* z){ return name.compare(z->objectName()) == 0; });
        auto zonePtr = *zoneIt;
        zonePtr->removeNeighborFromArray(this);
        neighbors.erase(zoneIt);

        emit Model::itemDisconnectedFromZone(this, zonePtr);
        emit Model::itemDisconnectedFromZone(zonePtr, this);
    }

    QJsonObject serialize() const override;
    void loadConnections(QJsonObject jsonObj, std::unordered_map<QString, Serializable *> addressTable) override;

    void visitedBy(ModelVisitor& visitor) override {
        visitor.visits(*this);
    }
};

#endif // PHYSICALZONE_H
