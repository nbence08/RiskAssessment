#ifndef ZONESEDIT_H
#define ZONESEDIT_H

#include <QWidget>

#include "model/physicalzone.hpp"
#include "model/logicalzone.hpp"
#include <vector>
#include <memory>

#include <QString>
#include <QInputDialog>
#include <QListWidget>
#include <QDir>
#include <QPen>
#include <QBrush>

namespace Ui {
    class ZonesEdit;
}

class ZonesEdit : public QDialog
{
    Q_OBJECT

public:
    explicit ZonesEdit(QWidget *parent = nullptr);
    ~ZonesEdit();

    void setPhysicalZones(std::vector<PhysicalZone*>* physicalZones){
        this->physicalZones = physicalZones;
    }

    void setLogicalZones(std::vector<LogicalZone*>* logicalZones){
        this->logicalZones = logicalZones;
    }

    QPen getPhysicalZoneColor(PhysicalZone* zone){
        if(!physicalZoneColors.contains(zone)){
            physicalZoneColors[zone] = QPen(getBrush(), 5);
        }
        return physicalZoneColors[zone];
    }

    QBrush getLogicalZoneColor(LogicalZone* zone){
        if(!logicalZoneColors.contains(zone)){
            logicalZoneColors[zone] = getBrush();
        }
        return logicalZoneColors[zone];
    }

    void resetColors(PhysicalZone* newGlobalPhys, LogicalZone* newGlobalLog){
        colorCounter = 0;
        physicalZoneColors.clear();
        logicalZoneColors.clear();

        physicalZoneColors[newGlobalPhys] = QPen(Qt::white, 5);
        logicalZoneColors[newGlobalLog] = QBrush(Qt::white);
    }

    //zonesEdit assumes the global zones are in position 0 in both vectors
    void configureGlobalZoneColors(){
        physicalZoneColors[physicalZones->at(0)] = QPen(Qt::white, 5);
        logicalZoneColors[logicalZones->at(0)] = QBrush(Qt::white);
    }

private slots:
    void on_actionExit_triggered();

    void on_phAdd_clicked();

    void on_phRemove_clicked();

    void on_logAdd_clicked();

    void on_logRemove_clicked();

protected:
    void closeEvent(QCloseEvent* event) override;


signals:
    void zoneEditExit();

private:
    Ui::ZonesEdit *ui;
    std::vector<PhysicalZone*>* physicalZones;
    std::vector<LogicalZone*>* logicalZones;

    int colorCounter;
    std::unordered_map<PhysicalZone*, QPen> physicalZoneColors;
    std::unordered_map<LogicalZone*, QBrush> logicalZoneColors;

    QListWidgetItem* selected;

    void itemClicked(QListWidgetItem *item);

    QBrush getBrush();
};

#endif // ZONESEDIT_H
