#ifndef SERIALIZATIONDISPATCHTABLEBUILDER_H
#define SERIALIZATIONDISPATCHTABLEBUILDER_H

#include "serializationdispatchtable.hpp"
#include <QObject>

class SerializationDispatchTableBuilder{
public:
    SerializationDispatchTableBuilder();

    static SerializationDispatchTable buildStandard(RiskEditController* controller);
private:
    static void loadCoordinates(QJsonObject& obj, QGraphicsItem& item){
        auto x = obj["x"].toDouble();
        auto y = obj["y"].toDouble();

        item.setX(x);
        item.setY(y);
    }

    template <class MODEL, class VIEW>
    static void addMVDispatch(SerializationDispatchTable& table, QString nameOfType){
        table.addDeserializationDispatch(nameOfType, [](RiskEditController& controller, QJsonObject obj){
            auto mvPair = controller.createMVPair<MODEL, VIEW>(obj["name"].toString());
            loadCoordinates(obj, *static_cast<QGraphicsItem*>(mvPair.second));

            return mvPair.first;
        });
    }
};

#endif // SERIALIZATIONDISPATCHTABLEBUILDER_H
