#ifndef PHYSICALZONEGRAPHICSVIEW_H
#define PHYSICALZONEGRAPHICSVIEW_H

#include <QGraphicsPixmapItem>
#include "diagramitem.hpp"
#include "viewvisitor.hpp"

class PhysicalZoneGraphicsView : public DiagramItem {
public:
    PhysicalZoneGraphicsView();
    inline PhysicalZoneGraphicsView(Model* model):DiagramItem(":/icons/PZone", model->objectName()){
        setModel(model);
    }

    void visitedBy(ViewVisitor& visitor) { visitor.visits(*this); }
};

#endif // PHYSICALZONEGRAPHICSVIEW_H
