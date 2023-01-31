#include "associationconnector.hpp"

#include "model/visitor.hpp"

AssociationConnector::AssociationConnector(QString label, QObject* parent):Connector(label, parent)
{

}

AssociationConnector::AssociationConnector(QGraphicsItem* start, QGraphicsItem* end, QString typeName, QObject* parent): Connector(start, end, typeName, parent) {
    connect(static_cast<DiagramItem*>(start)->getModel(), &Model::associationDeleted, this, &AssociationConnector::onAssociationDeleted);
}

void AssociationConnector::deleteElement() {
    auto startModel = static_cast<DiagramItem*>(begin)->getModel();
    auto endModel = static_cast<DiagramItem*>(end)->getModel();

    Visitor::DeleteAssociationByNameVisitor assocDeleter(endModel->objectName());
    startModel->visitedBy(assocDeleter);
}
