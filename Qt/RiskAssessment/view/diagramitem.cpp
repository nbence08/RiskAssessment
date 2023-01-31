#include "diagramitem.hpp"
#include "connector.cpp"

DiagramItem::DiagramItem(std::shared_ptr<Model> model, QString img, QString label):DiagramItem(img, label){
    setModel(model.get());

    createLabel(model->objectName());
}

DiagramItem::DiagramItem(QPixmap img, QString label){
    this->model = nullptr;

    icon = std::make_unique<QGraphicsPixmapItem>(img.scaled(QSize(DIAGRAM_ITEM_SIZE, DIAGRAM_ITEM_SIZE)), this);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    icon->setFlag(QGraphicsItem::ItemStacksBehindParent);

    createLabel(label);
}

DiagramItem::DiagramItem(QString img, QString label): DiagramItem(QPixmap(img), label){}

void DiagramItem::createLabel(QString label){
    auto name = std::make_unique<DiagramItemLabel>(icon.get());
    bg = std::make_unique<QGraphicsRectItem>(0, 0, DIAGRAM_ITEM_SIZE, DIAGRAM_ITEM_SIZE, this);
    bg->setBrush(Qt::white);
    QPen pen(Qt::white, 5);
    bg->setPen(pen);

    bg->setZValue(-1);
    bg->setFlag(QGraphicsItem::ItemStacksBehindParent);
    icon->setOpacity(1.0);

    icon->setShapeMode(QGraphicsPixmapItem::BoundingRectShape);

    name->setPlainText(label + "_VIEW");
    name->setDefaultTextColor(Qt::black);
    name->setTextInteractionFlags(Qt::TextEditorInteraction);

    auto x = icon->boundingRect().center().x() - name->boundingRect().width()/2.0;
    auto b = icon->boundingRect().bottom() + 1.0f;

    name->setPos(QPointF(x, b));
    this->label = std::move(name);

    connect((DiagramItemLabel*)this->label.get(), &DiagramItemLabel::nameChanged, this, &DiagramItem::nameChanged);
    this->label->centerTextBox();
}

DiagramItem::DiagramItem(QObject *parent):QObject(parent){
    this->model = nullptr;

    icon = std::make_unique<QGraphicsPixmapItem>(this);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    icon->setFlag(QGraphicsItem::ItemStacksBehindParent);
}

bool DiagramItem::connectedTo(DiagramItem* item){
    //return model->connectTo(item->getModel());
    this->getModel()->requestCreateAssociationToModel(*item->getModel());
    return true;
}

bool DiagramItem::grantedLPrivTo(DiagramItem* item){
    this->getModel()->requestGrantLPrivVisit(*item->getModel());
    return true;
}

bool DiagramItem::grantedConnectionTo(DiagramItem* item){
    this->getModel()->requestGrantConnectionVisit(*item->getModel());
    return true;
}

bool DiagramItem::grantedPAccessTo(DiagramItem* item){
    this->getModel()->requestGrantPAccessVisit(*item->getModel());
    return true;
}

bool DiagramItem::connectedToZone(DiagramItem* item){
    this->getModel()->requestConnectToZoneVisit(*item->getModel());
    return true;
}

Model* DiagramItem::getModel() {
    return model;
};

void DiagramItem::setModel(Model* model){
    if(model == this->model)
        return;
    if(this->model != nullptr){
        QObject::disconnect(this->model, &Model::modelDeleted, this, &DiagramItem::modelDeleted);
    }
    if(model != nullptr){
        QObject::connect(model, &Model::modelDeleted, this, &DiagramItem::modelDeleted);
    }
    this->model = model;

    auto modelName = model->objectName();
    auto viewName = modelName + "_VIEW";

    setObjectName(viewName);
    label->setPlainText(modelName);
    label->centerTextBox();
}

DiagramItem::~DiagramItem(){
    emit viewDeleted(this);
}

void DiagramItem::removeConnector(Connector* conn){
    auto connIt = std::find(connectors.begin(), connectors.end(), conn);

    if(connIt != connectors.end()){
        connectors.erase(connIt);
        disconnect(this, &DiagramItem::itemShown, conn, &Connector::endPointShown);
        disconnect(this, &DiagramItem::itemHidden, conn, &Connector::endPointHidden);
    }
}

void DiagramItem::addConnector(Connector* conn){
    auto connIt = std::find(connectors.begin(), connectors.end(), conn);

    if(connIt == connectors.end()){
        connectors.push_back(conn);
        connect(this, &DiagramItem::itemShown, conn, &Connector::endPointShown);
        connect(this, &DiagramItem::itemHidden, conn, &Connector::endPointHidden);
    }
}

void DiagramItem::modelDeleted(Model* model){
    if(this->model == model){
        delete this;
    }
}

void DiagramItem::deleteElement(){
    for(auto conn : std::vector<Connector*>(connectors)){
        conn->deleteElement();
    }

    delete this->model;
}

void DiagramItem::toggleHide(){
    if(hidden){
        icon->show();
        emit itemShown(this);
    }
    else {
        icon->hide();
        emit itemHidden(this);
    }
}

void DiagramItem::nameChanged(QString name){
    model->makeName(name);
    setObjectName(model->objectName()+"_VIEW");

    label->setPlainText(model->objectName());
    label->centerTextBox();
}
