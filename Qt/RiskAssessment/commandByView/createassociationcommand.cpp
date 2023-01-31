#include "createassociationcommand.hpp"
#include "riskeditcontroller.hpp"


void CreateAssociationCommand::execute(){
    descriptor.controller.createAssociationFromTo(descriptor.from.getModel(), descriptor.to.getModel());
}
