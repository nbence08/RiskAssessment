#ifndef ASSOCIATIONCONNECTOR_HPP
#define ASSOCIATIONCONNECTOR_HPP

#include "connector.hpp"

class AssociationConnector : public Connector
{
public:
    explicit AssociationConnector(QString label = "Association", QObject *parent = nullptr);
    AssociationConnector(QGraphicsItem* start, QGraphicsItem* end, QString typeName = "Association", QObject* parent = nullptr);

    void visitedBy(ViewVisitor& visitor) {
        visitor.visits(*this);
    };

    void deleteElement() override;
};

#endif // ASSOCIATIONCONNECTOR_HPP
