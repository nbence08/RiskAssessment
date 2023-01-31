#include "paccessconnector.hpp"

PAccessConnector::PAccessConnector(QString label, QObject *parent)
    : Connector{label, parent}
{

}

PAccessConnector::PAccessConnector(QGraphicsItem* start, QGraphicsItem* end, QString typeName, QObject* parent): Connector(start, end, typeName, parent) {
    connect(static_cast<DiagramItem*>(start)->getModel(), &Model::pAccessDeleted, this, &PAccessConnector::onPAccessDeleted);
}

void PAccessConnector::deleteElement() {
    auto beginModel = static_cast<DiagramItem*>(begin)->getModel();
    auto endModel = static_cast<DiagramItem*>(end)->getModel();

    Visitor::RemovePAccessByPzoneNameVisitor  paccDeleter(endModel->objectName());
    beginModel->visitedBy(paccDeleter);
}
