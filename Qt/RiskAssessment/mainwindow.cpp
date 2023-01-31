#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include "view/devicegraphicsview.hpp"

#include "model/allmodels.hpp"
#include "view/allviews.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , zonesEdit(this)
    , rolesDialog(this)
    , entitiesDialog(riskEditController, this)
{

    ui->setupUi(this);
    scene = std::make_unique<RiskEditScene>(this);
    scene->setMvPairs(&riskEditController.getMvPairs());
    riskEditController.setRiskEditScene(scene.get());

    ui->editArea->setScene(scene.get());
    scene->setSceneRect(0, 0, 700, 500);
    connect(scene.get(), &RiskEditScene::focusItemChanged, this, &MainWindow::sceneFocusItemChanged);

    auto glZonePair = riskEditController.createMVPair<LogicalZone,LogicalZoneGraphicsView>("global");
    auto gpZonePair = riskEditController.createMVPair<PhysicalZone, PhysicalZoneGraphicsView>("global");

    auto glZoneModel = glZonePair.first;
    auto glZoneView = glZonePair.second;

    auto gpZoneModel = gpZonePair.first;
    auto gpZoneView = gpZonePair.second;

    glZoneView->setPos(QPointF(200.0f, 10.0f));
    gpZoneView->setPos(QPointF(300.0f, 10.0f));

    globalLogZone = glZoneModel;
    globalPhysZone = gpZoneModel;

    /*zonesEdit.configureGlobalZoneColors();
    actualizeZones();

    glZoneView->setBackgroundColor(zonesEdit.getLogicalZoneColor(glZoneModel));
    gpZoneView->setBackgroundColor(zonesEdit.getPhysicalZoneColor(gpZoneModel).brush());*/

    currentFile = "";
    rolesDialog.setController(&riskEditController);
    connect(&rolesDialog, &RolesDialog::dialogExecutedCmd, this, &MainWindow::cmdExecuted);
}

MainWindow::~MainWindow(){
    this->riskEditController.clear();
    this->entitiesDialog.close();
    this->rolesDialog.close();

    delete ui;
}

/*
 * only kept around for remembering how to print to Qt output
 * can be deleted, if programmer knows how to do it
auto model = std::make_shared<QStandardItemModel>();

model->appendRow(new QStandardItem(1));
if(!model->setData(model->index(0,0), QVariant(10))){
   qWarning() << "Could not set data\n";
}
int var = model->data(model->index(0,0)).toInt();

qInfo() << var << '\n';*/

void MainWindow::on_connectModeButton_clicked(){
    scene->setMode(InputMode::Connect);
}

void MainWindow::on_selectModeButton_clicked(){
    scene->setMode(InputMode::Select);
}

void MainWindow::on_actionDelete_triggered(){
    auto items = scene->selectedItems();
    riskEditController.removeItems(items);
}


void MainWindow::on_actionExit_triggered(){
    this->entitiesDialog.close();
    this->rolesDialog.close();
    this->zonesEdit.close();
    this->close();
}

void MainWindow::sceneFocusItemChanged(QGraphicsItem *newFocusItem, QGraphicsItem*, Qt::FocusReason){}


void MainWindow::on_actionEdit_Zones_triggered(){
    zonesEdit.show();
}

void MainWindow::cmdExecuted(Command* cmd){
    cmdStack.push(std::unique_ptr<Command>(cmd));
}

void MainWindow::on_actionSave_triggered(){
    //QDir::home().dirName()

    if(currentFile == ""){
        auto savePath = QFileDialog::getSaveFileName(this, "Save");
        if(savePath == "")
            return;
        currentFile = savePath;
    }
    RiskSerializator saver;
    saver.save(riskEditController, currentFile);
}


void MainWindow::on_actionOpen_triggered(){
    RiskSerializator loader;
    auto loadPath = QFileDialog::getOpenFileName(this, "Load");
    if(loadPath == "")
        return;
    currentFile = loadPath;

    loader.load(riskEditController, loadPath);
    //zonesEdit.resetColors(riskEditController.getGlobalPhysicalZone(), riskEditController.getGlobalLogicalZone());
}


void MainWindow::on_actionSave_As_triggered(){
    auto savePath = QFileDialog::getSaveFileName(this, "Save", QDir::currentPath());
    if(savePath == "")
        return;
    currentFile = savePath;

    RiskSerializator saver;
    saver.save(riskEditController, currentFile);
}



void MainWindow::on_createDevice_clicked(){
    setAddItemLambda<Device, DeviceGraphicsView>();
}


void MainWindow::on_createActor_clicked(){
    setAddItemLambda<Actor, ActorGraphicsView>();
}


void MainWindow::on_createEmployee_clicked(){
    setAddItemLambda<Employee, EmployeeGraphicsView>();
}

void MainWindow::on_createOperation_clicked(){
    setAddItemLambda<Operation, OperationGraphicsView>();
}

void MainWindow::on_createLZone_clicked(){

    setAddItemLambda<LogicalZone, LogicalZoneGraphicsView>();
}


void MainWindow::on_createPZone_clicked(){
    setAddItemLambda<PhysicalZone, PhysicalZoneGraphicsView>();
}


void MainWindow::on_createService_clicked(){
    setAddItemLambda<Service, ServiceGraphicsView>();
}


void MainWindow::on_grantPAccess_clicked()
{
    setGrantPAccessLambda();
    scene->setMode(InputMode::Connect);
}


void MainWindow::on_grantConnection_clicked()
{
    setGrantConnectionLambda();
    scene->setMode(InputMode::Connect);
}


void MainWindow::on_grantLPriv_clicked()
{
    setGrantLPrivLambda();
    scene->setMode(InputMode::Connect);
}


void MainWindow::on_connectWithZone_clicked()
{
    setConnectWithZoneLambda();
    scene->setMode(InputMode::Connect);
}


void MainWindow::on_connectTo_clicked()
{
    setCreateAssociationLambda();
    scene->setMode(InputMode::Connect);
}


void MainWindow::setConnectWithZoneLambda(){
    setConnectLambda<ConnectWithZoneCommand>();
}

void MainWindow::setGrantConnectionLambda(){
    setConnectLambda<GrantConnectionCommand>();
}

void MainWindow::setGrantLPrivLambda(){
    setConnectLambda<GrantLPrivCommand>();
}

void MainWindow::setGrantPAccessLambda(){
    setConnectLambda<GrantPAccessCommand>();
}

void MainWindow::setCreateAssociationLambda()
{
    setConnectLambda<CreateAssociationCommand>();
}

void MainWindow::on_actionEdit_Roles_triggered()
{
    rolesDialog.reload();
    rolesDialog.show();
}


void MainWindow::on_createRole_clicked()
{
    setAddItemLambda<Role, RoleGraphicsView>();
}


void MainWindow::on_actionEdit_Entity_Privs_triggered(){
    entitiesDialog.reload();
    entitiesDialog.show();
}


void MainWindow::on_createServiceClass_clicked(){
    setAddItemLambda<ServiceClass, ServiceClassGraphicsView>();
}


void MainWindow::on_toggleConnLabelHide_clicked(){
    riskEditController.toggleConnectorLabelsHidde();
    riskEditController.visitViews(ToggleConnectorShownVisitor());
}

void MainWindow::on_actionNew_triggered(){
    currentFile = "";
    riskEditController.clear();
    auto lzPair = riskEditController.createMVPair<LogicalZone,LogicalZoneGraphicsView>("global");
    auto pzPair = riskEditController.createMVPair<PhysicalZone, PhysicalZoneGraphicsView>("global");

    lzPair.second->setPos(QPointF(200.0f, 10.0f));
    pzPair.second->setPos(QPointF(300.0f, 10.0f));
}
