#ifndef LOGICALZONE_H
#define LOGICALZONE_H

#include <vector>
#include <set>

#include "model.hpp"

class Device;
class PhysicalZone;

class LogicalZone : public Model {
public:
    static int idCount;
    explicit LogicalZone(QObject* parent = nullptr);
    LogicalZone(QString name, QObject* parent = nullptr);

    QJsonObject serialize() const override;
    void loadConnections(QJsonObject jsonObj, std::unordered_map<QString, Serializable*> addressTable) override;

    void visitedBy(ModelVisitor& visitor) override;
};

#endif // LOGICALZONE_H
