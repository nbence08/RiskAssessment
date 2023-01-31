#ifndef CONNECTOR_H
#define CONNECTOR_H

class DiagramItem;
class Connector;

#include <QObject>
#include <QString>
#include <QGraphicsLineItem>
#include <QPainter>

#include "viewvisitor.hpp"
#include "diagramitem.hpp"
#include "deletablediagramelement.hpp"

class Connector : public QObject, public QGraphicsLineItem, public DeletableDiagramElement {
    Q_OBJECT
protected:
    QGraphicsItem* begin;
    QGraphicsItem* end;

    std::shared_ptr<QGraphicsTextItem> connectTypeLabel;
    Model* model;

    QString typeName;
    short hiddenEndpointsCounter = 0;
protected:
    virtual QString getTypeName(){
        return typeName;
    }
public:
    Connector(QString typeName = "Connector", QObject* parent = nullptr):QObject(parent), typeName(typeName){
        connectTypeLabel = std::make_shared<QGraphicsTextItem>(this);
        connectTypeLabel->setPlainText(getTypeName());
    }
    Connector(QGraphicsItem* start, QGraphicsItem* end, QString typeName = "Connector", QObject* parent = nullptr);
    virtual ~Connector(){
        auto d1 = static_cast<DiagramItem*>(begin);
        auto d2 = static_cast<DiagramItem*>(end);

        d1->removeConnector(this);
        d2->removeConnector(this);

        emit connectorDeleted(this);
    }

    void hideLabel(){
        connectTypeLabel->hide();
    }

    void showLabel(){
        connectTypeLabel->show();
    }

    void toggleHideLabel(){
        if(connectTypeLabel->isVisible()){
            hideLabel();
        }
        else {
            showLabel();
        }
    }

    void setModel(Model* model);

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

    inline QGraphicsItem* getBegin(){ return begin; }
    inline QGraphicsItem* getEnd(){ return end; }

    void deleteElement() override;

    virtual void visitedBy(ViewVisitor& visitor) { visitor.visits(*this); }

signals:
    void connectorDeleted(Connector* connector);

public slots:
    void endPointShown(DiagramItem*){
        if(hiddenEndpointsCounter == 0){
            show();
        }
        else if (--hiddenEndpointsCounter > 0){
            return;
        }
    }
    void endPointHidden(DiagramItem*){
        if(hiddenEndpointsCounter == 2){
            return;
        }
        else if(++hiddenEndpointsCounter > 0){
            hide();
        }
    }

    virtual void onConnectionDeleted(Model* from, Model* to){
        if(from == static_cast<DiagramItem*>(begin)->getModel() && to == static_cast<DiagramItem*>(end)->getModel()){
            delete this;
        }
    }
    virtual void onAssociationDeleted(Model* from, Model* to){
        if(from == static_cast<DiagramItem*>(begin)->getModel() && to == static_cast<DiagramItem*>(end)->getModel()){
            delete this;
        }
    }
    virtual void onItemDisconnectedFromZone(Model* item, Model* zone){
        if(item == static_cast<DiagramItem*>(begin)->getModel() && zone == static_cast<DiagramItem*>(end)->getModel()){
            delete this;
        }
    }
    virtual void onLprivDeleted(Model* item, Model* operation){
        if(item == static_cast<DiagramItem*>(begin)->getModel() && operation == static_cast<DiagramItem*>(end)->getModel()){
            delete this;
        }
    }
    virtual void onPAccessDeleted(Model* item, Model* pzone){
        if(item == static_cast<DiagramItem*>(begin)->getModel() && pzone == static_cast<DiagramItem*>(end)->getModel()){
            delete this;
        }
    }
};

#endif // CONNECTOR_H
