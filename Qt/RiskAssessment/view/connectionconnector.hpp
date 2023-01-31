#ifndef CONNECTIONCONNECTOR_HPP
#define CONNECTIONCONNECTOR_HPP

#include "connector.hpp"

class ConnectionConnector : public Connector
{
public:
    explicit ConnectionConnector(QString label = "Connection", QObject *parent = nullptr);
    ConnectionConnector(DiagramItem* start, DiagramItem* end, QString typeName = "Connection", QObject* parent = nullptr);

    void deleteElement() override;

    void visitedBy(ViewVisitor& visitor) override {
        visitor.visits(*this);
    };
};

#endif // CONNECTIONCONNECTOR_HPP
