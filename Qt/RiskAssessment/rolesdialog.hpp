#ifndef ROLESDIALOG_HPP
#define ROLESDIALOG_HPP

#include <unordered_map>

#include <QDialog>

#include "core/command.hpp"
#include "model/role.hpp"
#include "model/lpriv.hpp"

class RiskEditController;

namespace Ui {
class RolesDialog;
}

class RolesDialog : public QDialog
{
    Q_OBJECT
private:
    QString currentRole;
    RiskEditController* controller;

    std::unordered_map<QString, Role*> roles;
    std::unordered_map<QString, std::weak_ptr<LPriv>> privs;

    std::vector<std::unique_ptr<Command>> commands;
public:
    explicit RolesDialog(QWidget *parent = nullptr);
    void setController(RiskEditController* controller){
        this->controller = controller;
    }
    ~RolesDialog();

    void reload();

    void flushCmds(){
        for(auto& cmd:commands){
            cmd->execute();
            dialogExecutedCmd(cmd.get());
        }
        commands.clear();
    }

signals:
    void dialogExecutedCmd(Command* cmd);

private slots:
    void on_addRoleButton_clicked();
    void on_cancelButton_clicked();
    void on_okButton_clicked();
    void on_removePrivFromRole_clicked();
    void on_addPrivToRole_clicked();
    void on_roles_itemSelectionChanged();

private:
    Ui::RolesDialog* ui;
    friend class LPrivLikeFillerVisitor;
    friend class RoleFillerVisitor;
};


class LPrivLikeFillerVisitor : public ModelVisitor {
protected:
    virtual void defaultOperation(Model&) override {}
    RolesDialog& dialog;
public:
    LPrivLikeFillerVisitor(RolesDialog& dialog):dialog(dialog){}

    void visits(Operation& op) override;
    void visits(Role& role) override;
};


#endif // ROLESDIALOG_HPP
