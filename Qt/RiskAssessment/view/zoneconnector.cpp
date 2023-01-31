#include "zoneconnector.hpp"

ZoneConnector::ZoneConnector(QString label, QObject *parent)
    : Connector{label, parent}
{

}

ZoneConnector::ZoneConnector(QGraphicsItem* start, QGraphicsItem* end, QString typeName, QObject* parent): Connector(start, end, typeName, parent) {
    connect(static_cast<DiagramItem*>(start)->getModel(), &Model::itemDisconnectedFromZone, this, &ZoneConnector::onItemDisconnectedFromZone);
}

void ZoneConnector::deleteElement() {
    auto beginModel = static_cast<DiagramItem*>(begin)->getModel();
    auto endModel = static_cast<DiagramItem*>(end)->getModel();

    Visitor::DisconnectItemFromZoneByNameVisitor  zoneDisconnector(endModel->objectName());
    beginModel->visitedBy(zoneDisconnector);
}
