#include "connectionconnector.hpp"
#include "diagramitem.hpp"

ConnectionConnector::ConnectionConnector(QString label, QObject *parent)
    : Connector{label, parent}
{

}

ConnectionConnector::ConnectionConnector(DiagramItem* start, DiagramItem* end, QString typeName, QObject* parent): Connector(start, end, typeName, parent) {
    connect(static_cast<DiagramItem*>(start)->getModel(), &Model::connectionDeleted, this, &ConnectionConnector::onConnectionDeleted);
}

void ConnectionConnector::deleteElement() {
    auto beginModel = static_cast<DiagramItem*>(begin)->getModel();
    auto endModel = static_cast<DiagramItem*>(end)->getModel();

    Visitor::RemoveConnectionByServiceNameVisitor connDeleter(endModel->objectName());
    beginModel->visitedBy(connDeleter);
}
