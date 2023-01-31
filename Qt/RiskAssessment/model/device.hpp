#ifndef DEVICE_H
#define DEVICE_H

#include <set>

#include "model.hpp"
#include "visitor.hpp"

class Service;
class LogicalZone;
class PhysicalZone;

class Device : public Model {
private:
    std::set<Service*> services;
    LogicalZone* lZone;
    PhysicalZone* pZone;

    class DeviceConnectWithZoneVisitor: public ModelVisitor {
    public:
        DeviceConnectWithZoneVisitor(Model* device);
        virtual void visits(LogicalZone& lZone) override;
        virtual void visits(PhysicalZone& pZone) override;
    };

    class DeviceAddServiceVisitor: public ModelVisitor {
    private:
        Device& device;
    public:
        DeviceAddServiceVisitor(Device& device);

        virtual void visits(Service& service);
    };

protected:
    /*virtual QString getNextID() override {
        return QString::number(idCounter++);
    }*/
public:
    explicit Device(QObject *parent = nullptr);
    Device(QString name, QObject* parent = nullptr);

    virtual QJsonObject serialize() const override;
    virtual void loadConnections(QJsonObject jsonObj, std::unordered_map<QString, Serializable*> addressTable) override;

    std::unique_ptr<ModelVisitor> getConnectToZoneVisitor() override {
        return std::make_unique<DeviceConnectWithZoneVisitor>(this);
    }

    std::unique_ptr<ModelVisitor> getCreateAssociationToVisitor() override{
        return std::make_unique<DeviceAddServiceVisitor>(*this);
    }

    void visitedBy(ModelVisitor& visitor) override {
        visitor.visits(*this);
    }

    friend class DeviceConnectWithZoneVisitor;
    friend class DeviceAddServiceVisitor;

    //signal emitter methods
    void removeServiceByName(QString name);
    void disconnectZoneByName(QString name);
};

#endif // DEVICE_H
