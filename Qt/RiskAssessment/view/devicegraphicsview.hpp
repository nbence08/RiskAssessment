#ifndef DEVICEGRAPHICSVIEW_H
#define DEVICEGRAPHICSVIEW_H

#include "diagramitem.hpp"
#include "viewvisitor.hpp"
#include "model/device.hpp"
#include "deletablediagramelement.hpp"

class DeviceGraphicsView : public DiagramItem {
public:

    DeviceGraphicsView(QString img = ":/icons/device", QString label = "device"):DiagramItem(img, label) {};
    DeviceGraphicsView(std::shared_ptr<Model> model, QString img = ":/icons/device", QString label = "device"):DiagramItem(model, img, label) {};

    void showProperties(Ui::MainWindow* ui) override;
    void loadRoles(Ui::MainWindow* ui);

    void visitedBy(ViewVisitor& visitor) { visitor.visits(*this); }

    inline virtual ~DeviceGraphicsView(){}


};

#endif // DEVICEGRAPHICSVIEW_H
