#ifndef RISKEDITCONTROLLER_H
#define RISKEDITCONTROLLER_H

#include <vector>
#include <memory>
#include <QObject>
#include <unordered_map>

#include "model/operation.hpp"
#include "model/role.hpp"
#include "model/service.hpp"
#include "model/visitor.hpp"
#include "view/riskeditscene.hpp"
#include "view/logicalzonegraphicsview.hpp"
#include "view/physicalzonegraphicsview.hpp"
#include "view/rolegraphicsview.hpp"

class DeviceGraphicsView;
class RiskEditScene;
class DiagramItem;
class ResourceGraphicsView;
class LogicalZoneGraphicsView;
class PhysicalZoneGraphicsView;
class LogicalZone;
class PhysicalZone;



class RiskEditController: public QObject{
    Q_OBJECT
private:

    std::unordered_map<Model*, DiagramItem*> mvPairs;
    std::unordered_map<const QGraphicsItem*, DiagramItem*> iconToView;
    std::unordered_map<const QGraphicsItem*, DeletableDiagramElement*> deleteTable;
    std::vector<DiagramItem*> itemViews;
    std::vector<Connector*> connectorViews;

    RiskEditScene* riskEditScene;

    bool connectorLabelsHidden;

    template<typename CONNECTOR>
    void createConnectorFromTo(Model* from, Model* to, QString label);
public:
    inline RiskEditController(QObject* parent = nullptr):QObject(parent) {
        connectorLabelsHidden = false;
        riskEditScene = nullptr;
    }
    RiskEditController(RiskEditScene* res, QObject* parent = nullptr):QObject(parent){
        riskEditScene = res;
        connectorLabelsHidden = false;
    }
    ~RiskEditController(){
        clear();
    }

    DiagramItem* getDiagramItemForIcon(const QGraphicsItem* item){
        return iconToView.at(item);
    }

    void setConnectorLabelsHidden(bool value){
        connectorLabelsHidden = value;
    }

    void toggleConnectorLabelsHidde(){
        connectorLabelsHidden = !connectorLabelsHidden;
    }

    void createRole(QString name);
    void destroyRoleByName(QString name){
        //TODO implement method
    }

    void clear();
    void removeView(DiagramItem* view);

    inline DiagramItem* getViewForModel(Model* model){
        return mvPairs.at(model);
    }

    void removeItems(QList<QGraphicsItem*> items);

    auto getMvPairs()-> decltype((mvPairs));
    void setMvPairs(const std::unordered_map<Model*, DiagramItem*>& newMvPairs);

    std::vector<LogicalZone*>& getLogicalZones();
    std::vector<PhysicalZone*>& getPhysicalZones();

    RiskEditScene *getRiskEditScene() const;
    void setRiskEditScene(RiskEditScene *newRiskEditScene);;

    std::pair<LogicalZone*, LogicalZoneGraphicsView*> createLogicalZone();
    std::pair<PhysicalZone*, PhysicalZoneGraphicsView*> createPhysicalZone();

    template<typename Visitor>
    void visitModels(Visitor& visitor){
        for(auto pair:mvPairs){
            pair.first->visitedBy(visitor);
        }
    }

    template<typename Visitor>
    void visitModels(Visitor&& visitor){
        for(auto pair:mvPairs){
            pair.first->visitedBy(visitor);
        }
    }

    void visitViews(ViewVisitor& visitor){
        for(auto pair:mvPairs){
            pair.second->visitedBy(visitor);
        }

        for(auto connector:connectorViews){
            connector->visitedBy(visitor);
        }
    }

    void visitViews(ViewVisitor&& visitor){
        for(auto pair:mvPairs){
            pair.second->visitedBy(visitor);
        }

        for(auto connector:connectorViews){
            connector->visitedBy(visitor);
        }
    }

    std::vector<DiagramItem*> getDeviceViews(){
        return itemViews;
    }

    void grantPAccessFromTo(Model* from, Model* to){
        from->requestGrantPAccessVisit(*to);
    }

    void grantLPrivFromTo(Model* from, Model* to){
        from->requestGrantLPrivVisit(*to);
    }

    void grantConnectionFromTo(Model* from, Model* to){
        from->requestGrantConnectionVisit(*to);
    }

    void connectItemToZone(Model* item, Model* zone){
        item->requestConnectToZoneVisit(*zone);
    }

    void createAssociationFromTo(Model* from, Model* to){
        from->requestCreateAssociationToModel(*to);
    }

    class GetLPrivForOperationVisitor : public ModelVisitor {
    private:
        QString opID;
        std::weak_ptr<LPriv> lpriv;
    public:
        GetLPrivForOperationVisitor(QString opID):opID(opID) {}

        void visits(Operation& op) override {
            lpriv = op.getLPriv();
        }

        std::weak_ptr<LPriv> getFoundLPriv(){
            if(lpriv.lock().get() == nullptr){
                throw std::out_of_range("lpriv not found for operation, operation does not exist");
            }
            return lpriv;
        }
    };

    template <typename Entity>
    void addOperationLPrivToEntity(Operation& op, Entity& entity){
        entity.requestGrantLPrivVisit(op);
    }

    class AddRoleToEntityByIDVisitor: public ModelVisitor {
    private:
        Role& role;
        QString entityID;

        template <typename T>
        void addRoleToEntity(T& entity){
            role.forEachLPriv([&entity](std::weak_ptr<LPriv> priv){
                entity.addLPriv(priv);
            });
        }
    public:
        AddRoleToEntityByIDVisitor(Role& role, QString entityID): role(role), entityID(entityID) {}

        void visits(Actor& actor);

        void visits(Employee& employee);
    };

    void addRoleToEntity(QString selectedRoleID, QString selectedEntityID);
    void removeRoleFromEntity(QString /*selectedRoleID*/, QString /*selectedEntityID*/){
        //TODO: implement
    }

public slots:
    void connectionMade(Model* start, Model* end);
    void onPAccessGranted(Model* from, Model* to);
    void onLPrivGranted(Model* from, Model* lPriv);
    void onConnectionAddedFromTo(Model* from, Model* to);
    void onItemConnectedToZone(Model* item, Model* zone);
    void onModelAssociatedToModel(AssociationData data);

    void onViewDeleted(QGraphicsItem* item);
    void onConnectorDeleted(Connector *conn);
    void onModelDeleted(Model* model);

public:
    template<typename MODEL, typename VIEW>
    std::pair<MODEL*, VIEW*> createMVPair(QString name = "");
signals:
    void hidePhysicalZones();
    void hideLogicalZones();
};

template <typename MODEL, typename VIEW>
std::pair<MODEL*, VIEW*> RiskEditController::createMVPair(QString name){
    MODEL* model = new MODEL(name);
    VIEW* view = new VIEW();

    view->setModel(model);

    if(riskEditScene != nullptr){
        riskEditScene->addItem(view);
    }
    mvPairs.emplace(model, view);
    iconToView[view->getIcon()] = view;

    //future improvement: abstract event handler registrations from here, to something more scalable, and sustainable
    connect(model, &Model::pAccessGranted, this, &RiskEditController::onPAccessGranted);
    connect(model, &Model::lPrivGranted, this, &RiskEditController::onLPrivGranted);
    connect(model, &Model::connectionAddedFromTo, this, &RiskEditController::onConnectionAddedFromTo);
    connect(model, &Model::itemConnectedToZone, this, &RiskEditController::onItemConnectedToZone);
    connect(model, &Model::modelAssociatedToModel, this, &RiskEditController::onModelAssociatedToModel);

    connect(model, &Model::modelDeleted, this, &RiskEditController::onModelDeleted);

    return {model, view};
}

template <typename CONNECTOR>
void RiskEditController::createConnectorFromTo(Model* from, Model* to, QString label){
    for(auto i = begin(connectorViews); i != connectorViews.end(); ++i){
        auto existingBegin = static_cast<DiagramItem*>((*i)->getBegin())->getModel();
        auto existingEnd = static_cast<DiagramItem*>((*i)->getEnd())->getModel();

        if((existingBegin == from && existingEnd == to) || (existingBegin == to && existingEnd == from)){
            return;
        }
    }

    auto startGrItem = getViewForModel(from);
    auto endGrItem = getViewForModel(to);

    auto connector = new CONNECTOR(startGrItem, endGrItem, label, this);
    connect(connector, &CONNECTOR::connectorDeleted, this, &RiskEditController::onConnectorDeleted);

    startGrItem->addConnector(connector);
    endGrItem->addConnector(connector);
    connector->setZValue(-1000);
    if(riskEditScene != nullptr){
        riskEditScene->addItem(connector);
    }
    connectorViews.push_back(connector);

    if(connectorLabelsHidden){
        connector->hideLabel();
    }
}


#endif // RISKEDITCONTROLLER_H
