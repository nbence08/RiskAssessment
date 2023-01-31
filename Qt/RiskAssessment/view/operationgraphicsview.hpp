#ifndef OPERATIONGRAPHICSVIEW_HPP
#define OPERATIONGRAPHICSVIEW_HPP

#include "diagramitem.hpp"
#include "viewvisitor.hpp"

class OperationGraphicsView : public DiagramItem {
public:
    OperationGraphicsView(QString img = ":/icons/operation", QString label = "operation"):DiagramItem(img, label) {};
    OperationGraphicsView(std::shared_ptr<Model> model, QString img = ":/icons/operation", QString label = "operation"):DiagramItem(model, img, label) {};

    void visitedBy(ViewVisitor& visitor) { visitor.visits(*this); }

    inline virtual ~OperationGraphicsView(){}
};

#endif // OPERATIONGRAPHICSVIEW_HPP
