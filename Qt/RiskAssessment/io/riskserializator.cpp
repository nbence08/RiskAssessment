#include "riskserializator.hpp"

#include "view/actorgraphicsview.hpp"
#include "view/employeegraphicsview.hpp"
#include "view/physicalzonegraphicsview.hpp"
#include "view/logicalzonegraphicsview.hpp"
#include "view/rolegraphicsview.hpp"
#include "view/devicegraphicsview.hpp"
#include "view/servicegraphicsview.hpp"
#include "view/operationgraphicsview.hpp"

RiskSerializator::RiskSerializator(){}

void RiskSerializator::save(RiskEditController& controller, QString path){
    QJsonDocument scene;

    QJsonArray jsonObjects;
    std::unordered_map<QString, QJsonArray> typeArrays;

    auto pairs = controller.getMvPairs();
    for(auto i = begin(pairs); i != end(pairs); i++){
        auto jsonModel = i->first->serialize();
        jsonModel["x"] = i->second->x();
        jsonModel["y"] = i->second->y();
        jsonModel["z"] = i->second->zValue();
        jsonObjects.append(jsonModel);
    }

    for(auto obj:jsonObjects){
        typeArrays[obj.toObject()["type"].toString()].append(obj);
    }

    QJsonObject sceneObject;
    for(auto pair:typeArrays){
        sceneObject[pair.first] = pair.second;
    }
    scene.setObject(sceneObject);

    QByteArray serialized = scene.toJson();

    QFile file(path);
    file.open(QIODevice::WriteOnly);
    file.write(serialized);
    file.close();
}

void RiskSerializator::load(RiskEditController& controller, QString path){
    controller.clear();

    QFile file(path);
    file.open(QIODevice::ReadOnly);

    QByteArray byteArr = file.readAll();

    QJsonDocument scene = QJsonDocument::fromJson(byteArr);
    QJsonObject typeArrays = scene.object();

    QJsonArray sceneArr;

    for(auto key:typeArrays.keys()){
        for(auto obj: typeArrays[key].toArray()){
            sceneArr.append(obj);
        }
    }

    createTypedObjects(controller, sceneArr);
}

void RiskSerializator::loadCoordinates(QJsonObject* obj, QGraphicsItem* item){
    auto x = (*obj)["x"].toDouble();
    auto y = (*obj)["y"].toDouble();

    item->setX(x);
    item->setY(y);
}

void RiskSerializator::createTypedObjects(RiskEditController& controller, QJsonArray& arr){
    std::unordered_map<QString, QJsonObject> objects;
    std::unordered_map<QString, Serializable*> serializables;

    auto table = SerializationDispatchTableBuilder::buildStandard(&controller);
    for(auto el:arr){
        QJsonObject obj = el.toObject();

        auto serializable = table.deserialize(obj);

        auto name = obj["name"].toString();
        auto type = obj["type"].toString();

        /*if(type == "LogicalZone" && name == "global"){
            serializables["global_logical"] = serializable;
        }
        else if(type == "PhysicalZone" && name == "global"){
            serializables["global_physical"] = serializable;
        }
        else*/
        serializables[name] = serializable;

        objects[name] = obj;
    }
    for(auto serPair:serializables){
        auto toBeLoadedSer = serPair.second;
        toBeLoadedSer->loadConnections(objects[serPair.first], serializables);
    }

    //postprocessing of names
    controller.visitViews(NameCorrectorVisitor());

}

void RiskSerializator::NameCorrectorVisitor::refreshName(DiagramItem& item){
    item.nameChanged(item.getModel()->objectName().split("_").last());
}

void RiskSerializator::NameCorrectorVisitor::visits(ActorGraphicsView& view){
    refreshName(view);
}

void RiskSerializator::NameCorrectorVisitor::visits(DeviceGraphicsView& view){
    refreshName(view);
}

void RiskSerializator::NameCorrectorVisitor::visits(EmployeeGraphicsView& view){
    refreshName(view);
}

void RiskSerializator::NameCorrectorVisitor::visits(LogicalZoneGraphicsView& view){
    refreshName(view);
}

void RiskSerializator::NameCorrectorVisitor::visits(OperationGraphicsView& view){
    refreshName(view);
}

void RiskSerializator::NameCorrectorVisitor::visits(PhysicalZoneGraphicsView& view){
    refreshName(view);
}

void RiskSerializator::NameCorrectorVisitor::visits(RoleGraphicsView& view){
    refreshName(view);
}

void RiskSerializator::NameCorrectorVisitor::visits(ServiceGraphicsView& view){
    refreshName(view);
}
