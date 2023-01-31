#include "connector.hpp"
#include "qpen.h"

Connector::Connector(QGraphicsItem* start, QGraphicsItem* end, QString typeName, QObject* parent):QObject(parent), typeName(typeName){
    this->begin = start;
    this->end = end;
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);

    connectTypeLabel = std::make_shared<QGraphicsTextItem>(this);
    connectTypeLabel->setPlainText(typeName);

    auto model = static_cast<DiagramItem*>(start)->getModel();
    connect(model, &Model::connectionDeleted, this, &Connector::onConnectionDeleted);
    connect(model, &Model::associationDeleted, this, &Connector::onAssociationDeleted);
    connect(model, &Model::itemDisconnectedFromZone, this, &Connector::onItemDisconnectedFromZone);
    connect(model, &Model::lprivDeleted, this, &Connector::onLprivDeleted);
    connect(model, &Model::pAccessDeleted, this, &Connector::onPAccessDeleted);
}

void Connector::setModel(Model *model){
    this->model = model;
}

//paint function based on https://doc.qt.io/qt-6/qtwidgets-graphicsview-diagramscene-example.html
void Connector::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    QPen myPen = pen();
    myPen.setColor(Qt::black);

    painter->setPen(myPen);
    painter->setBrush(Qt::black);

    auto bRect = begin->boundingRect();
    auto eRect = end->boundingRect();

    auto tl = end->pos() + eRect.topLeft();
    auto bl = end->pos() + eRect.bottomLeft();
    auto br = end->pos() + eRect.bottomRight();
    auto tr = end->pos() + eRect.topRight();

    QVector<QLineF> lines = {QLineF(tl, bl),
                             QLineF(bl, br),
                             QLineF(br, tr),
                             QLineF(tr, tl)};

    auto centerLine = QLineF(begin->pos() + bRect.center(), end->pos() + eRect.center());

    auto tAngle = -centerLine.angle();
    auto p1ToP2 = (centerLine.p2() - centerLine.p1());
    p1ToP2 /= sqrt(QPointF::dotProduct(p1ToP2, p1ToP2));
    auto tSize = connectTypeLabel->boundingRect().width();

    connectTypeLabel->textWidth();

    auto tPos = centerLine.center() - p1ToP2 * tSize/2.0;

    connectTypeLabel->setPos(tPos);
    connectTypeLabel->setRotation(tAngle);

    QPointF intersectionPoint;
    for(auto& line : lines){
        auto intersection = centerLine.intersects(line, &intersectionPoint);
        if(intersection == QLineF::BoundedIntersection){
            break;
        }
    }

    setLine(QLineF(centerLine.p1(), intersectionPoint));
    painter->drawLine(line());

    if (isSelected()) {
        painter->setPen(QPen(Qt::black, 1, Qt::DashLine));
        QLineF myLine = line();
        myLine.translate(0, 4.0);
        painter->drawLine(myLine);
        myLine.translate(0,-8.0);
        painter->drawLine(myLine);
    }
}

void Connector::deleteElement(){

}
