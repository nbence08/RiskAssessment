#include "visitor.hpp"

#include "allmodels.hpp"
#include "path.hpp"

void SearchModelIDVisitor::setModelIfIDEqualsToWantedName(Model& model){
    if(model.objectName().compare(wantedName) == 0){
        this->model = &model;
    }
}

void SearchModelIDVisitor::visits(Actor& actor) { setModelIfIDEqualsToWantedName(actor); }

void SearchModelIDVisitor::visits(Connection& connection) { setModelIfIDEqualsToWantedName(connection); }

void SearchModelIDVisitor::visits(Device& device) { setModelIfIDEqualsToWantedName(device); }

void SearchModelIDVisitor::visits(Employee& employee) { setModelIfIDEqualsToWantedName(employee); }

void SearchModelIDVisitor::visits(LogicalZone& lzone) { setModelIfIDEqualsToWantedName(lzone); }

void SearchModelIDVisitor::visits(LPriv& lpriv) { setModelIfIDEqualsToWantedName(lpriv); }

void SearchModelIDVisitor::visits(Operation& operation) { setModelIfIDEqualsToWantedName(operation); }

void SearchModelIDVisitor::visits(PAccess& paccess) { setModelIfIDEqualsToWantedName(paccess); }

void SearchModelIDVisitor::visits(PhysicalZone& pzone) { setModelIfIDEqualsToWantedName(pzone); }

void SearchModelIDVisitor::visits(Role& role) { setModelIfIDEqualsToWantedName(role); }

void SearchModelIDVisitor::visits(Service& service) { setModelIfIDEqualsToWantedName(service); }

void SearchModelIDVisitor::visits(ServiceClass& sc) { setModelIfIDEqualsToWantedName(sc); }

Model& SearchModelIDVisitor::getModel(){
    if(model == nullptr){
        throw std::out_of_range("model is nullptr");
    }

    return *model;
}

void ModelVisitor::defaultOperation(Model& /*model*/){ throw std::logic_error("Cannot perform action."); }

ModelVisitor::ModelVisitor(Model* model):model(model){}

ModelVisitor::ModelVisitor():model(nullptr){}

void ModelVisitor::visits(Actor& actor) { defaultOperation(actor); }

void ModelVisitor::visits(Connection& connection) { defaultOperation(connection); }

void ModelVisitor::visits(Device& device) { defaultOperation(device); }

void ModelVisitor::visits(Employee& employee) { defaultOperation(employee); }

void ModelVisitor::visits(LogicalZone& lzone) { defaultOperation(lzone); }

void ModelVisitor::visits(LPriv& lpriv) { defaultOperation(lpriv); }

void ModelVisitor::visits(Operation& operation) { defaultOperation(operation); }

void ModelVisitor::visits(PAccess& paccess) { defaultOperation(paccess); }

void ModelVisitor::visits(Path& path) { defaultOperation(path); }

void ModelVisitor::visits(PhysicalZone& pzone) { defaultOperation(pzone); }

void ModelVisitor::visits(Role& role) { defaultOperation(role); }

void ModelVisitor::visits(Service& service) { defaultOperation(service); }

void ModelVisitor::visits(ServiceClass& sc) { defaultOperation(sc); }

void FindModelsByIDVisitor::defaultOperation(Model& model) {
    for(auto& name:modelsToFind){
        if(model.objectName().compare(name) == 0){
            modelsFound[name] = &model;
        }
    }
}

FindModelsByIDVisitor::FindModelsByIDVisitor(std::vector<QString> modelsToFind):modelsToFind(modelsToFind) {}

Model* FindModelsByIDVisitor::operator[](QString& name){
    return modelsFound.at(name);
}

void Visitor::RemoveLPrivByOperationNameVisitor::visits(Actor& actor){
    removeLPriv(actor);
}

void Visitor::RemoveLPrivByOperationNameVisitor::visits(Employee& emp){
    removeLPriv(emp);
}

void Visitor::RemoveLPrivByOperationNameVisitor::visits(Role& role){
    role.removeLPrivForOperation(this->name);
}

void Visitor::RemoveLPrivByOperationNameVisitor::visits(Operation& op){
    op.removeLPrivTo(this->name);
}

void Visitor::RemovePAccessByPzoneNameVisitor::visits(Actor& actor){
    actor.removePAccessTo(name);
}

void Visitor::RemovePAccessByPzoneNameVisitor::visits(Employee& emp){
    emp.removePAccessTo(name);
}

void Visitor::RemovePAccessByPzoneNameVisitor::visits(Operation& op){
    op.removePAccessTo(name);
}

void Visitor::RemoveConnectionByServiceNameVisitor::visits(Actor& actor){
    actor.removeConnectionTo(name);
}

void Visitor::RemoveConnectionByServiceNameVisitor::visits(Employee& emp){
    emp.removeConnectionTo(name);
}

void Visitor::RemoveConnectionByServiceNameVisitor::visits(Operation& op){
    op.removeConnectionTo(name);
}

void Visitor::DeleteAssociationByNameVisitor::visits(Device& dev){
    dev.removeServiceByName(name);
}

void Visitor::DeleteAssociationByNameVisitor::visits(Service& service){
    service.removeOperationByName(name);
}

void Visitor::DeleteAssociationByNameVisitor::visits(ServiceClass& serviceClass){
    serviceClass.removeServiceByName(name);
}

void Visitor::DisconnectItemFromZoneByNameVisitor::visits(Device& dev){
    dev.disconnectZoneByName(name);
}

void Visitor::DisconnectItemFromZoneByNameVisitor::visits(PhysicalZone& zone){
    zone.removeNeighborByName(name);
}
