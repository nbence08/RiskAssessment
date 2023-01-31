#ifndef ZONECONNECTOR_HPP
#define ZONECONNECTOR_HPP

#include "connector.hpp"

class ZoneConnector : public Connector
{
public:
    explicit ZoneConnector(QString label = "InZone", QObject *parent = nullptr);
    ZoneConnector(QGraphicsItem* start, QGraphicsItem* end, QString typeName = "InZone", QObject* parent = nullptr);

    void visitedBy(ViewVisitor& visitor) override { visitor.visits(*this); }

    virtual void deleteElement() override;
};

#endif // ZONECONNECTOR_HPP
