#include "riskeditscene.hpp"
#include "qmessagebox.h"

RiskEditScene::RiskEditScene(QObject *parent)
    : QGraphicsScene{parent}
{
    mode = InputMode::Select;
}

void RiskEditScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    if(mode == InputMode::Connect){
        auto startPos = mouseEvent->scenePos();
        line = std::make_shared<QGraphicsLineItem>(QLineF(startPos, startPos));
        line->setPen(QPen(Qt::black, 1));
        addItem(line.get());

    }
    if(mode == InputMode::Select){
        QGraphicsScene::mousePressEvent(mouseEvent);
    }

    if(mode == InputMode::AddItem){
        auto pos = mouseEvent->scenePos();

        addItemLambda(pos.x(), pos.y());
    }
}

void RiskEditScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    if(mode == InputMode::Connect && line != nullptr){
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
    }
    if(mode == InputMode::Select){
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

void RiskEditScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) {

    if(mode == InputMode::Connect && line != nullptr){
        removeItem(line.get());
        auto startItems = items(line->line().p1());
        auto endItems = items(line->line().p2());
        line.reset();

        if(startItems.count() > 0 && endItems.count() > 0 &&
                startItems.first() != endItems.first()){
            DiagramItem* first = static_cast<DiagramItem*>(startItems.first());
            DiagramItem* end = static_cast<DiagramItem*>(endItems.first());

            try{
                connectItemLambda(first, end);
            }
            catch(std::logic_error& e){
                QMessageBox::warning(nullptr, "error", e.what());
            }
        }
    }
    if(mode == InputMode::Select){
        QGraphicsScene::mouseReleaseEvent(mouseEvent);
    }
}
