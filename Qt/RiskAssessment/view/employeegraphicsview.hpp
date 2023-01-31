#ifndef EMPLOYEEGRAPHICSVIEW_HPP
#define EMPLOYEEGRAPHICSVIEW_HPP

#include "diagramitem.hpp"
#include "viewvisitor.hpp"

class EmployeeGraphicsView : public DiagramItem{
public:
    EmployeeGraphicsView(QString img = ":/icons/employee", QString label = "employee"):DiagramItem(img, label) {};
    EmployeeGraphicsView(std::shared_ptr<Model> model, QString img = ":/icons/employee", QString label = "employee"):DiagramItem(model, img, label) {};

    void visitedBy(ViewVisitor& visitor) { visitor.visits(*this); }

    inline virtual ~EmployeeGraphicsView(){}

};

#endif // EMPLOYEEGRAPHICSVIEW_HPP
