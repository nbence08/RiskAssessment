#ifndef LPRIVCONNECTOR_HPP
#define LPRIVCONNECTOR_HPP

#include "connector.hpp"

class LPrivConnector : public Connector {
public:
    explicit LPrivConnector(QString label = "HasLPriv", QObject *parent = nullptr);
    LPrivConnector(QGraphicsItem* start, QGraphicsItem* end, QString typeName = "Connection", QObject* parent = nullptr);

    void visitedBy(ViewVisitor& visitor) { visitor.visits(*this); }

    virtual void deleteElement() override;
};

#endif // LPRIVCONNECTOR_HPP
