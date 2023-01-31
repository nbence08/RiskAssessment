#include "entitiesdialog.hpp"
#include "ui_entitiesdialog.h"

#include "model/actor.hpp"
#include "model/employee.hpp"

EntitiesDialog::EntitiesDialog(RiskEditController& controller, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::EntitiesDialog),
    controller(controller){
    ui->setupUi(this);
    this->setWindowTitle("Edit Entity LPrivs");
}

EntitiesDialog::~EntitiesDialog(){
    delete ui;
}

void EntitiesDialog::on_removePrivFromEntitiyButton_clicked(){
    if(ui->entitiesList->selectedItems().empty()){
        return;
    }
    auto toBeRemovedWidget = ui->privsInEntityList->selectedItems().first();
    auto toBeRemovedName = toBeRemovedWidget->text();
    auto entity = entities[ui->entitiesList->selectedItems().first()->text()];

    Visitor::RemoveLPrivByOperationNameVisitor privDeleter(toBeRemovedName);
    entity->visitedBy(privDeleter);

    ui->privsInEntityList->removeItemWidget(toBeRemovedWidget);
    delete toBeRemovedWidget;
    ui->privsList->addItem(toBeRemovedName);
}

void EntitiesDialog::on_addPrivToEntityButton_clicked(){
    if(ui->entitiesList->selectedItems().empty()){
        return;
    }
    auto toBeAddedWidget = ui->privsList->selectedItems().first();
    auto toBeAddedName = toBeAddedWidget->text();
    auto& toBeAddedModel = lprivs[toBeAddedName].lock()->getOperation();
    auto entity = entities[ui->entitiesList->selectedItems().first()->text()];

    entity->requestGrantLPrivVisit(toBeAddedModel);

    ui->privsList->removeItemWidget(toBeAddedWidget);
    delete toBeAddedWidget;
    ui->privsInEntityList->addItem(toBeAddedName);
}

void EntitiesDialog::on_removeRoleFromEntityButton_clicked(){
    if(ui->entitiesList->selectedItems().empty()){
        return;
    }
    auto toBeRemovedWidget = ui->rolesInEntityList->selectedItems().first();
    auto toBeRemovedName = toBeRemovedWidget->text();
    auto entity = entities[ui->entitiesList->selectedItems().first()->text()];

    Visitor::RemoveLPrivByOperationNameVisitor roleDeleter(toBeRemovedName, true);
    entity->visitedBy(roleDeleter);

    ui->rolesInEntityList->removeItemWidget(toBeRemovedWidget);
    delete toBeRemovedWidget;
    ui->rolesList->addItem(toBeRemovedName);
}

void EntitiesDialog::on_addRoleToEntityButton_clicked(){
    if(ui->entitiesList->selectedItems().empty()){
        return;
    }
    auto toBeAddedWidget = ui->rolesList->selectedItems().first();
    auto toBeAddedName = toBeAddedWidget->text();
    auto& toBeAddedModel = roles[toBeAddedName];
    auto entity = entities[ui->entitiesList->selectedItems().first()->text()];

    entity->requestGrantLPrivVisit(*toBeAddedModel);

    ui->rolesInEntityList->addItem(toBeAddedName);
    ui->rolesList->removeItemWidget(toBeAddedWidget);
    delete toBeAddedWidget;
}

void EntitiesDialog::EntitiesListFillerVisitor::visits(Actor& actor){
    dialog.entities[actor.objectName()] = &actor;
    dialog.ui->entitiesList->addItem(actor.objectName());
}

void EntitiesDialog::EntitiesListFillerVisitor::visits(Employee& emp){
    dialog.entities[emp.objectName()] = &emp;
    dialog.ui->entitiesList->addItem(emp.objectName());
}

void EntitiesDialog::EntitiesListFillerVisitor::visits(Role& role){
    dialog.roles[role.objectName()] = &role;
    dialog.ui->rolesList->addItem(role.objectName());
}

void EntitiesDialog::EntitiesListFillerVisitor::visits(Operation& operation){
    dialog.lprivs[operation.objectName()] = operation.getLPriv();
    dialog.ui->privsList->addItem(operation.objectName());
}

void EntitiesDialog::LPrivLikeListFiller::visits(Actor& actor){
    visits<Actor>(actor);
}

void EntitiesDialog::LPrivLikeListFiller::visits(Employee& emp){
    visits<Employee>(emp);
}

void EntitiesDialog::on_entitiesList_itemSelectionChanged()
{
    if(processingEntitiesListEvent){
        return;
    }
    processingEntitiesListEvent = true;
    reload();
    processingEntitiesListEvent = false;
}

