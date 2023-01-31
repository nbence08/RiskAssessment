#include "rolesdialog.hpp"
#include "ui_rolesdialog.h"

#include "riskeditcontroller.hpp"
#include <QInputDialog>
#include <QMessageBox>
#include "model/employee.hpp"
#include "model/actor.hpp"
#include "model/visitor.hpp"
#include "model/role.hpp"
#include "model/lpriv.hpp"

RolesDialog::RolesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RolesDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Edit Roles");
}

RolesDialog::~RolesDialog()
{
    delete ui;
}

void RolesDialog::reload()
{
    ui->roles->clear();
    ui->lPrivsInRole->clear();
    ui->allPrivs->clear();

    privs.clear();
    roles.clear();

    LPrivLikeFillerVisitor visitor(*this);

    controller->visitModels(visitor);
}

void RolesDialog::on_addRoleButton_clicked()
{
    bool ok;
    auto roleName = QInputDialog::getText(this, tr("new role nam"), tr("New Role Name:"), QLineEdit::Normal, "newRole", &ok);

    if(ok && !roleName.isEmpty()){
        ui->roles->addItem(roleName);

        auto command = std::make_unique<LambdaCommand>([controller=this->controller, roleName=roleName](){
            controller->createRole(roleName);
        },
        [controller=this->controller, roleName=roleName](){
            controller->destroyRoleByName(roleName);
        });

        ui->roles->addItem(roleName);
        commands.push_back(std::move(command));
    }
}

void RolesDialog::on_cancelButton_clicked()
{
    commands.clear();
    this->hide();
}


void RolesDialog::on_okButton_clicked()
{
    for(auto& cmd:commands){
        cmd->execute();
        emit dialogExecutedCmd(cmd.get());
    }
    commands.clear();
    this->hide();
}


class RemovePrivFromRoleVisitor : public ModelVisitor {
private:
    QString privID;
    QString roleID;
public:
    RemovePrivFromRoleVisitor(QString privID, QString roleID): privID(privID), roleID(roleID) {}

    void visits(Role& role) override {
        if(role.objectName().compare(roleID) == 0){
            try{
                role.removeLPrivForOperation(privID);
            }
            catch(std::out_of_range& e){
                QMessageBox::warning(nullptr, "warning", e.what());
            }
        }
    }
};

void RolesDialog::on_removePrivFromRole_clicked(){
    if(ui->roles->selectedItems().empty()){
        return;
    }
    if(!ui->lPrivsInRole->selectedItems().empty()){
        auto selectedLPrivWidget = ui->lPrivsInRole->selectedItems()[0];
        auto selectedRoleWidget =  ui->roles->selectedItems()[0];

        auto selectedOperationID = selectedLPrivWidget->text();
        auto selectedRoleID = selectedRoleWidget->text();

        auto selectedRole = roles[selectedRoleID];
        selectedRole->removeLPrivForOperation(selectedOperationID);

        ui->lPrivsInRole->removeItemWidget(selectedLPrivWidget);
        ui->allPrivs->addItem(selectedOperationID);

        delete selectedLPrivWidget;
    }
}

void RolesDialog::on_addPrivToRole_clicked(){
    if(ui->roles->selectedItems().empty()){
        return;
    }
    if(!ui->allPrivs->selectedItems().empty()){
        auto selectedPrivWidget = ui->allPrivs->selectedItems()[0];
        auto selectedRoleWidget = ui->roles->selectedItems()[0];

        auto selectedPrivID = selectedPrivWidget->text();
        auto selectedRoleID = selectedRoleWidget->text();

        auto selectedRole = roles[selectedRoleID];
        auto& selectedOperation = privs[selectedPrivID].lock()->getOperation();

        selectedRole->requestGrantLPrivVisit(selectedOperation);

        ui->allPrivs->removeItemWidget(selectedPrivWidget);
        ui->lPrivsInRole->addItem(selectedPrivID);

        delete selectedPrivWidget;
    }
}

void RolesDialog::on_roles_itemSelectionChanged(){
    auto selectedRoles = ui->roles->selectedItems();
    if(!selectedRoles.empty()){
        auto selectedRoleName = ui->roles->selectedItems().first()->text();
        auto selectedRole = roles[selectedRoleName];

        for(int i = ui->lPrivsInRole->count()-1; i > -1; --i){
            auto privWidget = ui->lPrivsInRole->item(i);
            auto opName = privWidget->text();

            delete privWidget;
            ui->allPrivs->addItem(opName);
        }
        selectedRole->forEachLPriv([this](std::weak_ptr<LPriv> priv){
            auto opName = priv.lock()->getOperation().objectName();

            auto foundItems = ui->allPrivs->findItems(opName, Qt::MatchExactly);
            for(auto item:foundItems){
                ui->lPrivsInRole->addItem(item->text());
                delete item;
            }
        });
    }
}

void LPrivLikeFillerVisitor::visits(Operation& op) {
    dialog.ui->allPrivs->addItem(op.objectName());
    dialog.privs[op.objectName()] = op.getLPriv();
}

void LPrivLikeFillerVisitor::visits(Role& role) {
    dialog.ui->roles->addItem(role.objectName());
    dialog.roles[role.objectName()] = &role;
}
