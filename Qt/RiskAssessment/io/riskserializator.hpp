#ifndef RISKSERIALIZATOR_H
#define RISKSERIALIZATOR_H

#include <QObject>
#include <QString>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QFile>

#include "riskeditcontroller.hpp"
#include "serializationdispatchtable.hpp"
#include "serializationdispatchtablebuilder.hpp"


class RiskSerializator{
public:
    RiskSerializator();

    void save(RiskEditController& storage, QString path);
    void load(RiskEditController& storage, QString path);

private:
    void createTypedObjects(RiskEditController& storage, QJsonArray& arr);
    void loadCoordinates(QJsonObject* obj, QGraphicsItem* item);

    SerializationDispatchTable* serDispatchTable;

    class NameCorrectorVisitor: public ViewVisitor {
    private:
        void refreshName(DiagramItem& item);

    public:
        void visits(ActorGraphicsView& view);
        void visits(DeviceGraphicsView& view);
        void visits(EmployeeGraphicsView& view);
        void visits(LogicalZoneGraphicsView& view);
        void visits(OperationGraphicsView& view);
        void visits(PhysicalZoneGraphicsView& view);
        void visits(RoleGraphicsView& view);
        void visits(ServiceGraphicsView& view);
    };
};

#endif // RISKSERIALIZATOR_H
