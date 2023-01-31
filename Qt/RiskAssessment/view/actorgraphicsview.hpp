#ifndef ACTORGRAPHICSVIEW_HPP
#define ACTORGRAPHICSVIEW_HPP

#include "viewvisitor.hpp"
#include "diagramitem.hpp"

class ActorGraphicsView : public DiagramItem {
public:
    ActorGraphicsView(QString img = ":/icons/actor", QString label = "actor"):DiagramItem(img, label) {};
    ActorGraphicsView(std::shared_ptr<Model> model, QString img = ":/icons/actor", QString label = "actor"):DiagramItem(model, img, label) {};

    inline virtual ~ActorGraphicsView(){}

    void visitedBy(ViewVisitor& visitor) { visitor.visits(*this); }
};

#endif // ACTORGRAPHICSVIEW_HPP
