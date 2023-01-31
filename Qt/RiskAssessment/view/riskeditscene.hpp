#ifndef RISKEDITSCENE_H
#define RISKEDITSCENE_H

#include <vector>

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLineItem>

#include "connector.hpp"
#include "view/diagramitem.hpp"
#include "core/command.hpp"

enum class InputMode { Select, Connect, AddItem};

class RiskEditScene : public QGraphicsScene
{
private:
    std::unique_ptr<Command> addItemCommand;
    std::function<void(float, float)> addItemLambda;

    std::unique_ptr<Command> connectItemCommand;
    std::function<void(DiagramItem*,DiagramItem*)> connectItemLambda;
public:
    explicit RiskEditScene(QObject *parent = nullptr);

    void setAddItemLambda(std::function<void(float, float)> addItemLambda){
        this->addItemLambda = addItemLambda;
    }

    void setConnectItemLambda(std::function<void(DiagramItem*, DiagramItem*)> connectItemLambda){
        this->connectItemLambda = connectItemLambda;
    }

    void setMode(InputMode newMode){
        mode = newMode;
    }

    inline InputMode getMode(){ return mode; }
    inline void setMvPairs(std::unordered_map<Model*, DiagramItem*>* ps){ mvPairs = ps; }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

private:
    InputMode mode;
    std::shared_ptr<QGraphicsLineItem> line;
    std::vector<Connector*> connections;
    std::unordered_map<Model*, DiagramItem*>* mvPairs;
};

#endif // RISKEDITSCENE_H
