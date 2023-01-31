#include "model.hpp"


Model::Model(QString namePrefix, QObject* parent):QObject(parent), namePrefix(namePrefix){
    makeName();
}
Model::Model(QString name, QString namePrefix, QObject* parent):Model(namePrefix, parent) { makeName(name); }

Model::~Model() {
    emit modelDeleted(this);
}

void Model::makeName(QString newName){
    if(newName == ""){
        newName = getNameType() + idNum;
    }
    if(idNum.isEmpty()){
        modelGetNextID();
    }
    setObjectName(namePrefix + '_' + idNum + "_" + newName);
}

void Model::requestGrantPAccessVisit(Model& model){
    model.visitedBy(*this->getGrantPAccessVisitor());
}

void Model::requestGrantLPrivVisit(Model& model){
    model.visitedBy(*this->getGrantLPrivVisitor());
}

void Model::requestGrantConnectionVisit(Model& model){
    model.visitedBy(*this->getGrantConnectionVisitor());
}

void Model::requestConnectToZoneVisit(Model& model){
    model.visitedBy(*this->getConnectToZoneVisitor());
}

void Model::requestCreateAssociationToModel(Model& model){
    model.visitedBy(*this->getCreateAssociationToVisitor());
}
