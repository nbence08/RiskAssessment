#include "lprivconnector.hpp"

LPrivConnector::LPrivConnector(QString label, QObject *parent)
    : Connector{label, parent}
{

}

LPrivConnector::LPrivConnector(QGraphicsItem* start, QGraphicsItem* end, QString typeName, QObject* parent): Connector(start, end, typeName, parent) {
    connect(static_cast<DiagramItem*>(start)->getModel(), &Model::lprivDeleted, this, &LPrivConnector::onLprivDeleted);
}

void LPrivConnector::deleteElement() {
    auto beginModel = static_cast<DiagramItem*>(begin)->getModel();
    auto endModel = static_cast<DiagramItem*>(end)->getModel();

    Visitor::RemoveLPrivByOperationNameVisitor privDeleter(endModel->objectName(), true);
    beginModel->visitedBy(privDeleter);
}
