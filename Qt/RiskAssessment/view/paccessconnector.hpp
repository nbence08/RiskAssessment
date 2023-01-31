#ifndef PACCESSCONNECTOR_HPP
#define PACCESSCONNECTOR_HPP

#include "connector.hpp"

class PAccessConnector : public Connector
{
public:
    explicit PAccessConnector(QString label = "HasPAccess", QObject *parent = nullptr);
    PAccessConnector(QGraphicsItem* start, QGraphicsItem* end, QString typeName = "HasPAccess", QObject* parent = nullptr);

    void visitedBy(ViewVisitor& visitor) { visitor.visits(*this); }

    virtual void deleteElement() override;
};

#endif // PACCESSCONNECTOR_HPP
