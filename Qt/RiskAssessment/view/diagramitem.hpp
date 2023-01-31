#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include <QPolygonF>
#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>

#include "diagramitemlabel.hpp"
#include "model/model.hpp"

#include "ui_mainwindow.h"
#include <QLineEdit>
#include <QUuid>

#include <QObject>
#include <QMainWindow>
#include "viewvisitor.hpp"

class Connector;

#include "deletablediagramelement.hpp"

#define DIAGRAM_ITEM_SIZE 50

class DiagramItem: public QObject, public QGraphicsItem, public DeletableDiagramElement {
    Q_OBJECT
private:
    bool hidden = false;
    // creates a label under the DiagramElement
    void createLabel(QString label);

public:

    DiagramItem(QObject* parent = nullptr);
    DiagramItem(QString img, QString label);
    DiagramItem(std::shared_ptr<Model> model, QString img, QString label);
    DiagramItem(QPixmap img, QString label);
    virtual ~DiagramItem();

    virtual bool connectedTo(DiagramItem* item);
    virtual bool grantedLPrivTo(DiagramItem* item);
    virtual bool grantedConnectionTo(DiagramItem* item);
    virtual bool grantedPAccessTo(DiagramItem* item);
    virtual bool connectedToZone(DiagramItem* item);

    virtual Model* getModel();
    virtual void setModel(Model* model);
    virtual void showProperties(Ui::MainWindow*) {}

    inline QGraphicsRectItem& getBackground(){ return *bg; }
    inline void setBackgroundColor(QBrush br){ bg->setBrush(br); }
    inline void setBorderColor(QPen pen){ bg->setPen(pen); }
    QGraphicsItem* getIcon(){ return icon.get(); }

    void removeConnector(Connector* conn);
    void addConnector(Connector* conn);

    virtual void visitedBy(ViewVisitor& visitor) = 0;

public:
signals:
    void viewDeleted(DiagramItem*);
    void itemShown(DiagramItem*);
    void itemHidden(DiagramItem*);

public slots:
    virtual void modelDeleted(Model* model);
    virtual void deleteElement() override;
    void toggleHide();
    void nameChanged(QString name);


protected:
    std::vector<Connector*> connectors;
    std::unique_ptr<QGraphicsPixmapItem> icon;
    std::unique_ptr<DiagramItemLabel> label;
    std::unique_ptr<QGraphicsRectItem> bg;
    Model* model;


    // QGraphicsItem interface
public:
    QRectF boundingRect() const override {
        auto rect = icon->boundingRect();
        return {rect.left()-10, rect.top()-10, rect.width()+20, rect.height()+20};
    }
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override {
        icon->paint(painter, option, widget);
    }



    // QGraphicsItem interface
protected:
    /*QVariant itemChange(GraphicsItemChange change, const QVariant& value) override {
        if(change == ItemPositionChange){
            int abc = 0;
            abc;
        }
    }*/
};

#endif // DIAGRAMITEM_H
