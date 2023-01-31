#ifndef SERVICEGRAPHICSVIEW_HPP
#define SERVICEGRAPHICSVIEW_HPP

#include "diagramitem.hpp"
#include "viewvisitor.hpp"

class ServiceGraphicsView : public DiagramItem {
public:
    ServiceGraphicsView(QString img = ":/icons/service", QString label = "service"):DiagramItem(img, label) {};
    ServiceGraphicsView(std::shared_ptr<Model> model, QString img = ":/icons/service", QString label = "service"):DiagramItem(model, img, label) {};

    void visitedBy(ViewVisitor& visitor) { visitor.visits(*this); }

    inline virtual ~ServiceGraphicsView(){}
};

#endif // SERVICEGRAPHICSVIEW_HPP
