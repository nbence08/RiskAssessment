#include "grantconnectioncommand.hpp"
#include "riskeditcontroller.hpp"

void GrantConnectionCommand::execute(){
    descriptor.controller.grantConnectionFromTo(descriptor.from.getModel(), descriptor.to.getModel());
}
