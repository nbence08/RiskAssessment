#ifndef LOGICALZONEGRAPHICSVIEW_H
#define LOGICALZONEGRAPHICSVIEW_H

#include <QGraphicsPixmapItem>
#include "diagramitem.hpp"
#include "viewvisitor.hpp"
#include "model/logicalzone.hpp"

class LogicalZoneGraphicsView : public DiagramItem {
public:
    LogicalZoneGraphicsView();
    inline LogicalZoneGraphicsView(Model* model):DiagramItem(":/icons/LZone", model->objectName()){
        setModel(model);
    }

    void visitedBy(ViewVisitor& visitor) { visitor.visits(*this); }
};

#endif // LOGICALZONEGRAPHICSVIEW_H
