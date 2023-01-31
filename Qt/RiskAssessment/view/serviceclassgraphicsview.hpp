#ifndef SERVICECLASSGRAPHICSVIEW_HPP
#define SERVICECLASSGRAPHICSVIEW_HPP

#include "diagramitem.hpp"

class ServiceClassGraphicsView : public DiagramItem {
public:
    ServiceClassGraphicsView(QString img = ":/icons/serviceclass", QString label = "serviceclass"):DiagramItem(img, label) {};
    ServiceClassGraphicsView(std::shared_ptr<Model> model, QString img = ":/icons/serviceclass", QString label = "serviceclass"):DiagramItem(model, img, label) {};

    void visitedBy(ViewVisitor& visitor){
        visitor.visits(*this);
    }
};

#endif // SERVICECLASSGRAPHICSVIEW_HPP
