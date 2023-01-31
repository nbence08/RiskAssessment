#ifndef ENTITIESDIALOG_HPP
#define ENTITIESDIALOG_HPP

#include <QDialog>
#include <QObject>

#include "model/visitor.hpp"
#include "model/model.hpp"
#include "model/role.hpp"
#include "model/lpriv.hpp"
#include "riskeditcontroller.hpp"
#include "ui_entitiesdialog.h"

namespace Ui {
class EntitiesDialog;
}

class EntitiesDialog : public QDialog
{
    Q_OBJECT
private:
    Ui::EntitiesDialog* ui;
    RiskEditController& controller;

    bool processingEntitiesListEvent = false;

    //  relevant object caches
    std::unordered_map<QString, Model*> entities;
    std::unordered_map<QString, Role*> roles;
    std::unordered_map<QString, std::weak_ptr<LPriv>> lprivs;
    friend class EntitiesListFillerVisitor;

    class EntitiesListFillerVisitor : public ModelVisitor {
        EntitiesDialog& dialog;
    protected:
        virtual void defaultOperation(Model& model) override {}
    public:
        EntitiesListFillerVisitor(EntitiesDialog& dialog):dialog(dialog){}
        void visits(Actor& actor) override;
        void visits(Employee& emp) override;
        void visits(Role& role) override;
        void visits(Operation& operation) override;
    };

    class LPrivLikeListFiller : public ModelVisitor {
        QString objectName;
        EntitiesDialog& dialog;

        template <typename ENTITY>
        void visits(ENTITY& entity);
    protected:
        virtual void defaultOperation(Model& model) override {}
    public:
        LPrivLikeListFiller(QString objectName, EntitiesDialog& dialog):objectName(objectName),dialog(dialog) {}
        void visits(Actor& actor) override;
        void visits(Employee& emp) override;
    };

public:
    explicit EntitiesDialog(RiskEditController& controller, QWidget *parent = nullptr);
    ~EntitiesDialog();

    void reload(){

        entities.clear();
        roles.clear();
        lprivs.clear();

        QListWidgetItem* selectedEntity;
        QString selectedEntityName;
        auto selectedEntities = ui->entitiesList->selectedItems();
        if(!selectedEntities.isEmpty()){
            selectedEntity = selectedEntities.first();
            selectedEntityName = selectedEntity->text();
        }

        ui->entitiesList->clear();
        ui->privsList->clear();
        ui->privsInEntityList->clear();
        ui->rolesList->clear();
        ui->rolesInEntityList->clear();

        EntitiesListFillerVisitor entFiller(*this);
        controller.visitModels(entFiller);

        if(selectedEntities.empty()){
            return;
        }
        if(selectedEntityName.isEmpty()){
            return;
        }

        LPrivLikeListFiller privRoleFiller(selectedEntityName, *this);
        controller.visitModels(privRoleFiller);

        auto toBeSelected = ui->entitiesList->findItems(selectedEntityName, Qt::MatchExactly).first();
        toBeSelected->setSelected(true);
    }

    void refresh(){
        roles.clear();
        lprivs.clear();

        ui->privsList->clear();
        ui->privsInEntityList->clear();
        ui->rolesList->clear();
        ui->rolesInEntityList->clear();

        auto selectedEntity = ui->entitiesList->selectedItems().first()->text();
        if(!selectedEntity.isEmpty()){
            LPrivLikeListFiller privRoleFiller(selectedEntity, *this);
            controller.visitModels(privRoleFiller);
        }
    }

private slots:
    void on_removePrivFromEntitiyButton_clicked();
    void on_addPrivToEntityButton_clicked();
    void on_removeRoleFromEntityButton_clicked();
    void on_addRoleToEntityButton_clicked();
    void on_entitiesList_itemSelectionChanged();

    // QWidget interface
protected:
    void closeEvent(QCloseEvent* event) override {
        QDialog::closeEvent(event);

        ui->entitiesList->clear();
        ui->privsList->clear();
        ui->privsInEntityList->clear();
        ui->rolesList->clear();
        ui->rolesInEntityList->clear();
    }
};

template <typename ENTITY>
void EntitiesDialog::LPrivLikeListFiller::visits(ENTITY& entity){
    if(entity.objectName().compare(objectName) == 0){
        entity.foreachOwnedLPriv([this](LPriv& priv){
           dialog.ui->privsInEntityList->addItem(priv.getOperation().objectName());

           auto containedElement = dialog.ui->privsList->findItems(priv.getOperation().objectName(), Qt::MatchExactly);
           for(auto item:containedElement){
               delete item;
           }
        });

        entity.foreachRole([this](Role& role){
            dialog.ui->rolesInEntityList->addItem(role.objectName());

            auto containedElement = dialog.ui->rolesList->findItems(role.objectName(), Qt::MatchExactly);
            for(auto item:containedElement){
                delete item;
            }
        });
    }
}

#endif // ENTITIESDIALOG_HPP
