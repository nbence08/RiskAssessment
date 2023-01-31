#include "riskeditcontroller.hpp"
#include "model/logicalzone.hpp"
#include "model/physicalzone.hpp"
#include "model/role.hpp"
#include "model/employee.hpp"
#include "model/actor.hpp"
#include "view/rolegraphicsview.hpp"

#include "view/connectionconnector.hpp"
#include "view/associationconnector.hpp"
#include "view/lprivconnector.hpp"
#include "view/paccessconnector.hpp"
#include "view/zoneconnector.hpp"


void RiskEditController::createRole(QString name){
    createMVPair<Role, RoleGraphicsView>(name);
}

void RiskEditController::clear() {
    auto connsCopy = connectorViews;
    for(auto conn:connsCopy){
        conn->deleteElement();
    }

    auto mvPairsCopy = mvPairs;
    for(auto [model, view]:mvPairsCopy){
        delete model;
    }

    connectorViews.clear();
    mvPairs.clear();
    connectorLabelsHidden = false;
    Model::resetCounter();
}

void RiskEditController::removeView(DiagramItem* view){
    for(auto i = begin(itemViews); i != end(itemViews); i++){
        if(*i == view){
            itemViews.erase(i);
            return;
        }
    }
}

void RiskEditController::removeItems(QList<QGraphicsItem*> items){
    for(auto item:items){
        dynamic_cast<DeletableDiagramElement*>(item)->deleteElement();
    }
}

auto RiskEditController::getMvPairs() -> decltype((mvPairs)) {
    return mvPairs;
}

void RiskEditController::setMvPairs(const std::unordered_map<Model*, DiagramItem*> &newMvPairs)
{
    mvPairs = newMvPairs;
}

void RiskEditController::setRiskEditScene(RiskEditScene* newRiskEditScene) {
    this->riskEditScene = newRiskEditScene;
}

std::pair<LogicalZone*, LogicalZoneGraphicsView*> RiskEditController::createLogicalZone(){
    auto mvPair = createMVPair<LogicalZone, LogicalZoneGraphicsView>();

    connect(this, &RiskEditController::hideLogicalZones, mvPair.second, &LogicalZoneGraphicsView::toggleHide);
    return mvPair;
}

std::pair<PhysicalZone*, PhysicalZoneGraphicsView*> RiskEditController::createPhysicalZone(){
    auto mvPair = createMVPair<PhysicalZone, PhysicalZoneGraphicsView>();

    connect(this, &RiskEditController::hideLogicalZones, mvPair.second, &LogicalZoneGraphicsView::toggleHide);
    return mvPair;
}

void RiskEditController::addRoleToEntity(QString selectedRoleID, QString selectedEntityID){
    FindModelsByIDVisitor findModelsVisitor({selectedRoleID});
    visitModels(findModelsVisitor);

    Role* selectedRole = static_cast<Role*>(findModelsVisitor[selectedRoleID]);

    visitModels(AddRoleToEntityByIDVisitor(*selectedRole, selectedEntityID));
}

void RiskEditController::connectionMade(Model *start, Model *end){

    //this procedure does not account for the direction of a connection
    for(auto i = begin(connectorViews); i != connectorViews.end(); ++i ){
        auto existingBegin = static_cast<DiagramItem*>((*i)->getBegin())->getModel();
        auto existingEnd = static_cast<DiagramItem*>((*i)->getEnd())->getModel();

        if((existingBegin == start && existingEnd == end) || (existingBegin == end && existingEnd == start)){
            return;
        }
    }

    DiagramItem* startGrItem = getViewForModel(start);
    DiagramItem* endGrItem = getViewForModel(end);

    auto connect = new ConnectionConnector(startGrItem, endGrItem);

    startGrItem->addConnector(connect);
    endGrItem->addConnector(connect);
    connect->setZValue(-1000);
    riskEditScene->addItem(connect);
    //addConnector(connect);

    //TODO: add signal to connector which signals the controller, whenever the connector is deleted, so that it is removed from the controller's
    // connectorViews array
}

void RiskEditController::onPAccessGranted(Model* from, Model* to){
    createConnectorFromTo<PAccessConnector>(from, to, "HasPAccess");
}
void RiskEditController::onLPrivGranted(Model* from, Model* to){
    createConnectorFromTo<LPrivConnector>(from, to, "HasLPriv");
}
void RiskEditController::onConnectionAddedFromTo(Model* from, Model* to){
    createConnectorFromTo<ConnectionConnector>(from, to, "HasConnection");
}
void RiskEditController::onItemConnectedToZone(Model* item, Model* zone){
    createConnectorFromTo<ZoneConnector>(item, zone, "InZone");
}
void RiskEditController::onModelAssociatedToModel(AssociationData data){
    createConnectorFromTo<AssociationConnector>(data.from, data.to, data.name);
}

void RiskEditController::onViewDeleted(QGraphicsItem* view){
    auto mvIt = std::find_if(mvPairs.begin(), mvPairs.end(), [view](std::pair<Model*, QGraphicsItem*> pair){
        return pair.second == view;
    });

    if(mvIt != mvPairs.end()){
        delete mvIt->first;
        mvPairs.erase(mvIt);
    }
}

void RiskEditController::onConnectorDeleted(Connector* conn){
    for(auto i = begin(connectorViews); i != end(connectorViews); ++i){
        if(*i == conn){
            connectorViews.erase(i);
            break;
        }
    }
    disconnect(conn, &Connector::connectorDeleted, this, &RiskEditController::onConnectorDeleted);
}

void RiskEditController::onModelDeleted(Model* model){
    auto mvIt = std::find_if(mvPairs.begin(), mvPairs.end(), [model](std::pair<Model*, QGraphicsItem*> pair){
        return pair.first == model;
    });

    if(mvIt != mvPairs.end()){
        mvPairs.erase(mvIt);
    }
}

void RiskEditController::AddRoleToEntityByIDVisitor::visits(Actor& actor){
    if(actor.objectName().compare(entityID) == 0){
        this->addRoleToEntity(actor);
    }
}

void RiskEditController::AddRoleToEntityByIDVisitor::visits(Employee& employee){
    if(employee.objectName().compare(entityID) == 0){
        this->addRoleToEntity(employee);
    }
}
