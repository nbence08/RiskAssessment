#include "logicalzone.hpp"
#include "device.hpp"

LogicalZone::LogicalZone(QObject *parent)
    : Model{"LZ", parent}
{
}

LogicalZone::LogicalZone(QString name, QObject* parent): Model(name, "LZ", parent) {  }

int LogicalZone::idCount = 0;

QJsonObject LogicalZone::serialize() const {
    QJsonObject jsonLZone;
    jsonLZone["name"] = objectName();
    jsonLZone["type"] = "LogicalZone";

    return jsonLZone;
}

void LogicalZone::loadConnections(QJsonObject jsonObj, std::unordered_map<QString, Serializable *> addressTable){}

void LogicalZone::visitedBy(ModelVisitor& visitor) {
    visitor.visits(*this);
}
