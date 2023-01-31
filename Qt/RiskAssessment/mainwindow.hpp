#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QMainWindow>
#include <QVector>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QString>
#include <QLineEdit>
#include <QInputDialog>
#include <QtGlobal>
#include <QFileDialog>

#include <memory>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>
#include <stack>

#include "riskeditcontroller.hpp"
#include "io/riskserializator.hpp"

#include "view/devicegraphicsview.hpp"
#include "view/riskeditscene.hpp"
#include "commandByView/additemcommand.hpp"
#include "commandByView/connectwithzonecommand.hpp"
#include "commandByView/grantconnectioncommand.hpp"
#include "commandByView/grantlprivcommand.hpp"
#include "commandByView/grantpaccesscommand.hpp"
#include "commandByView/createassociationcommand.hpp"

#include "zonesedit.hpp"
#include "rolesdialog.hpp"
#include "entitiesdialog.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    template <typename MODEL, typename VIEW>
    void setAddItemLambda();

    template <typename ConnectCommand>
    void setConnectLambda();

    std::stack<std::unique_ptr<Command>> cmdStack;

    class ToggleConnectorShownVisitor : public ViewVisitor {
    public:
        void visits(Connector& connector) override {
            connector.toggleHideLabel();
        }
    };

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void sceneFocusItemChanged(QGraphicsItem *newFocusItem, QGraphicsItem *oldFocusItem, Qt::FocusReason reason);
    void cmdExecuted(Command* cmd);

private slots:
    void on_connectModeButton_clicked();
    void on_selectModeButton_clicked();
    void on_actionDelete_triggered();
    void on_actionExit_triggered();
    void on_actionEdit_Zones_triggered();

    void on_actionSave_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_As_triggered();
    void on_createDevice_clicked();
    void on_createActor_clicked();
    void on_createEmployee_clicked();
    void on_createLZone_clicked();
    void on_createPZone_clicked();
    void on_createService_clicked();
    void on_grantPAccess_clicked();
    void on_grantConnection_clicked();
    void on_grantLPriv_clicked();
    void on_connectWithZone_clicked();
    void on_connectTo_clicked();
    void on_createOperation_clicked();
    void on_actionEdit_Roles_triggered();
    void on_createRole_clicked();
    void on_actionEdit_Entity_Privs_triggered();
    void on_createServiceClass_clicked();
    void on_toggleConnLabelHide_clicked();
    void on_actionNew_triggered();

private:
    LogicalZone* globalLogZone;
    PhysicalZone* globalPhysZone;

    Ui::MainWindow* ui;

    RiskEditController riskEditController;

    std::unique_ptr<RiskEditScene> scene;

    void setInputModeAddLine(){
        scene->setMode(InputMode::Connect);
    }

    ZonesEdit zonesEdit;
    RolesDialog rolesDialog;
    EntitiesDialog entitiesDialog;

    QString currentFile;

    friend class DiagramItem;
    void setConnectWithZoneLambda();
    void setGrantConnectionLambda();
    void setGrantLPrivLambda();
    void setGrantPAccessLambda();
    void setCreateAssociationLambda();
};

template <typename MODEL, typename VIEW>
void MainWindow::setAddItemLambda(){
    scene->setMode(InputMode::AddItem);

    auto lambda = [this, &riskEditController = this->riskEditController](float x, float y){
        std::unique_ptr<Command> addItemCmd = std::make_unique<AddItemCommand<MODEL, VIEW>>(AddItemCommandDescriptor{x, y, riskEditController});
        addItemCmd->execute();
        cmdStack.push(std::move(addItemCmd));
    };

    scene->setAddItemLambda(lambda);
}

template <typename ConnectCommand>
void MainWindow::setConnectLambda(){
    auto lambda = [this, &riskEditController = this->riskEditController](DiagramItem* from, DiagramItem* to) {
        std::unique_ptr<Command> connectWithZoneCmd = std::make_unique<ConnectCommand>(ConnectCommandDescriptor{*from, *to, riskEditController});
        connectWithZoneCmd->execute();
        cmdStack.push(std::move(connectWithZoneCmd));
    };
    scene->setConnectItemLambda(lambda);
}

#endif // MAINWINDOW_H
