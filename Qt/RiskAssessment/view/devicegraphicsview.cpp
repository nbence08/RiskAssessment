#include "devicegraphicsview.hpp"

void DeviceGraphicsView::loadRoles(Ui::MainWindow* ui){
    /*auto device = static_cast<Device*>(getModel());

    for(auto role : device->getRoles()){
        addRoleView(*role, ui);
    }*/
}

void DeviceGraphicsView::showProperties(Ui::MainWindow* ui) {
    /*ui->opsPage->setDisabled(true);
    ui->rolesPage->setDisabled(false);
    ui->protectionsPage->setDisabled(false);
    ui->zonesPage->setDisabled(false);

    for(auto role : static_cast<Device*>(getModel())->getRoles()){
        addRoleView(*role, ui);
    }

    auto device = static_cast<Device*>(model);
    if(ui->physicalZoneBox->currentText() != device->getPhysicalZone()->objectName()){
        for(int i = 0; i < ui->physicalZoneBox->count(); i++){
            if(ui->physicalZoneBox->itemText(i) == device->getPhysicalZone()->objectName()){
                ui->physicalZoneBox->setCurrentIndex(i);
                break;
            }
        }
    }

    if(ui->logicalZoneBox->currentText() != device->getLogicalZone()->objectName()){
        for(int i = 0; i < ui->logicalZoneBox->count(); i++){
            if(ui->logicalZoneBox->itemText(i) == device->getLogicalZone()->objectName()){
                ui->logicalZoneBox->setCurrentIndex(i);
                break;
            }
        }
    }*/
}
