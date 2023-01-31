#include "serializationdispatchtablebuilder.hpp"
#include <QObject>

#include "../model/allmodels.hpp"
#include "../view/allviews.hpp"

SerializationDispatchTableBuilder::SerializationDispatchTableBuilder(){

}

void loadCoordinates(QJsonObject* obj, QGraphicsItem* item){
    auto x = (*obj)["x"].toDouble();
    auto y = (*obj)["y"].toDouble();

    item->setX(x);
    item->setY(y);
}

SerializationDispatchTable SerializationDispatchTableBuilder::buildStandard(RiskEditController* controller){

    SerializationDispatchTable table(controller);

    addMVDispatch<Actor, ActorGraphicsView>(table, "Actor");
    addMVDispatch<Device, DeviceGraphicsView>(table, "Device");
    addMVDispatch<Employee, EmployeeGraphicsView>(table, "Employee");
    addMVDispatch<LogicalZone, LogicalZoneGraphicsView>(table, "LogicalZone");
    addMVDispatch<Operation, OperationGraphicsView>(table, "Operation");
    addMVDispatch<PhysicalZone, PhysicalZoneGraphicsView>(table, "PhysicalZone");
    addMVDispatch<Role, RoleGraphicsView>(table, "Role");
    addMVDispatch<Service, ServiceGraphicsView>(table, "Service");
    addMVDispatch<ServiceClass, ServiceClassGraphicsView>(table, "ServiceClass");

    return table;
}
