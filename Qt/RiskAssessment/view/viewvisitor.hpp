#ifndef VIEWVISITOR_HPP
#define VIEWVISITOR_HPP

#include <qgraphicsitem.h>

class DiagramItem;
class ActorGraphicsView;
class Connector;
class DeviceGraphicsView;
class EmployeeGraphicsView;
class LogicalZoneGraphicsView;
class OperationGraphicsView;
class PhysicalZoneGraphicsView;
class RoleGraphicsView;
class ServiceGraphicsView;
class ServiceClassGraphicsView;

class ViewVisitor
{
private:
    std::function<void(QGraphicsItem&)> defaultOperation;
public:
    ViewVisitor(std::function<void(QGraphicsItem&)> defOp = [](QGraphicsItem&){}) { defaultOperation = defOp; };

    virtual void visits(ActorGraphicsView& view);
    virtual void visits(Connector& view);
    virtual void visits(DeviceGraphicsView& view);
    virtual void visits(EmployeeGraphicsView& view);
    virtual void visits(LogicalZoneGraphicsView& view);
    virtual void visits(OperationGraphicsView& view);
    virtual void visits(PhysicalZoneGraphicsView& view);
    virtual void visits(RoleGraphicsView& view);
    virtual void visits(ServiceGraphicsView& view);
    virtual void visits(ServiceClassGraphicsView& view);
};

#endif // VIEWVISITOR_HPP
