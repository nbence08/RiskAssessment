#ifndef ROLEGRAPHICSVIEW_HPP
#define ROLEGRAPHICSVIEW_HPP

#include "diagramitem.hpp"
#include "viewvisitor.hpp"

class RoleGraphicsView : public DiagramItem {
public:
    RoleGraphicsView(QString img = ":/icons/role", QString label = "role"):DiagramItem(img, label) {};
    RoleGraphicsView(std::shared_ptr<Model> model, QString img = ":/icons/role", QString label = "role"):DiagramItem(model, img, label) {};

    void visitedBy(ViewVisitor& visitor) { visitor.visits(*this); }
};

#endif // ROLEGRAPHICSVIEW_HPP
