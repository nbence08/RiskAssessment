#include "zonesedit.hpp"
#include "ui_zonesedit.h"

ZonesEdit::ZonesEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ZonesEdit){

    ui->setupUi(this);
    connect(ui->logicalZonesList, &QListWidget::itemClicked, this, &ZonesEdit::itemClicked);
    connect(ui->physicalZonesList, &QListWidget::itemClicked, this, &ZonesEdit::itemClicked);
    colorCounter = 0;

}

ZonesEdit::~ZonesEdit(){
    delete ui;
}

void ZonesEdit::on_actionExit_triggered(){
    this->hide();
}

void ZonesEdit::closeEvent(QCloseEvent*) {
    emit zoneEditExit();
}


void ZonesEdit::on_phAdd_clicked(){
    /*bool ok;
    QString zoneName = QInputDialog::getText(this, "New Physical Zone Name", "Name:", QLineEdit::Normal, "newPhysicalZone", &ok);

    if(ok){
        QListWidgetItem* newItem = new QListWidgetItem();
        newItem->setText(zoneName);
        ui->physicalZonesList->addItem(newItem);
        auto newZone = std::make_shared<PhysicalZone>();
        newZone->setObjectName(zoneName);
        physicalZones->push_back(newZone);

        physicalZoneColors[newZone.get()] = QPen(getBrush(), 5);
    }*/
}


void ZonesEdit::on_logAdd_clicked(){
    /*bool ok;
    QString zoneName = QInputDialog::getText(this, "New Logical Zone Name", "Name:", QLineEdit::Normal, "newLogicalZone", &ok);

    if(ok){
        QListWidgetItem* newItem = new QListWidgetItem();
        newItem->setText(zoneName);
        ui->logicalZonesList->addItem(newItem);
        auto newZone = std::make_shared<LogicalZone>();
        newZone->setObjectName(zoneName);
        logicalZones->push_back(newZone);

        logicalZoneColors[newZone.get()] = getBrush();
    }*/
}


void ZonesEdit::on_phRemove_clicked(){
    if(selected != nullptr){
        auto text = selected->text();

        ui->physicalZonesList->removeItemWidget(selected);
        delete selected;
        selected = nullptr;
        for(auto i = begin(*physicalZones); i != end(*physicalZones); i++){
            auto item = *i;
            if(item->objectName() == text){
                physicalZones->erase(i);
                return;
            }
        }

    }
}

void ZonesEdit::on_logRemove_clicked(){
    if(selected != nullptr){
        auto text = selected->text();

        ui->logicalZonesList->removeItemWidget(selected);
        delete selected;
        selected = nullptr;
        for(auto i = begin(*logicalZones); i != end(*logicalZones); i++){
            auto item = *i;
            if(item->objectName() == text){
                logicalZones->erase(i);
                return;
            }
        }

    }
}

void ZonesEdit::itemClicked(QListWidgetItem *item){
    this->selected = item;
}

QBrush ZonesEdit::getBrush(){
    if(colorCounter < 13){
        switch(colorCounter++){
        case 0:
            return QBrush(Qt::red);
        case 1:
            return QBrush(Qt::green);
        case 2:
            return QBrush(Qt::blue);
        case 3:
            return QBrush(Qt::cyan);
        case 4:
            return QBrush(Qt::magenta);
        case 5:
            return QBrush(Qt::yellow);
        case 6:
            return QBrush(Qt::darkRed);
        case 7:
            return QBrush(Qt::darkGreen);
        case 8:
            return QBrush(Qt::darkBlue);
        case 9:
            return QBrush(Qt::darkCyan);
        case 10:
            return QBrush(Qt::darkMagenta);
        case 11:
            return QBrush(Qt::darkYellow);
        case 12:
            return QBrush(Qt::lightGray);
        }
    }
    return QBrush(Qt::white);
}
