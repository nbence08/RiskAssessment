#ifndef SERIALIZATIONDISPATCHTABLE_H
#define SERIALIZATIONDISPATCHTABLE_H


#include <unordered_map>
#include <functional>
#include "riskeditcontroller.hpp"

#include <QObject>
#include <QString>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QFile>

#include "riskeditcontroller.hpp"

class SerializationDispatchTable {
using name_of_type_t = QString;
using dispatch_func_t = Serializable*(RiskEditController&, QJsonObject);

public:
    SerializationDispatchTable(RiskEditController* controller = nullptr):controller(controller) {};

    inline void addDeserializationDispatch(QString name, std::function<dispatch_func_t> func){
        deserializationDispatchTable[name] = func;
    }

    inline Serializable* deserialize(QJsonObject obj){
        auto type = obj["type"].toString();
        return deserializationDispatchTable[type](*controller, obj);
    }

private:
    RiskEditController* controller;
    std::unordered_map<name_of_type_t, std::function<dispatch_func_t>> deserializationDispatchTable;
};

#endif // SERIALIZATIONDISPATCHTABLE_H
