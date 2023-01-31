#include "viewvisitor.hpp"

#include "allviews.hpp"

void ViewVisitor::visits(ActorGraphicsView& view) { defaultOperation(view); }
void ViewVisitor::visits(Connector& view) { defaultOperation(view); }
void ViewVisitor::visits(DeviceGraphicsView& view) { defaultOperation(view); }
void ViewVisitor::visits(EmployeeGraphicsView& view) { defaultOperation(view); }
void ViewVisitor::visits(LogicalZoneGraphicsView& view) { defaultOperation(view); }
void ViewVisitor::visits(OperationGraphicsView& view) { defaultOperation(view); }
void ViewVisitor::visits(PhysicalZoneGraphicsView& view) { defaultOperation(view); }
void ViewVisitor::visits(RoleGraphicsView& view) { defaultOperation(view); }
void ViewVisitor::visits(ServiceGraphicsView& view) { defaultOperation(view); }
void ViewVisitor::visits(ServiceClassGraphicsView& view){ defaultOperation(view); }
