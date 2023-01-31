#include "grantpaccesscommand.hpp"
#include "riskeditcontroller.hpp"

void GrantPAccessCommand::execute() {
    descriptor.controller.grantPAccessFromTo(descriptor.from.getModel(), descriptor.to.getModel());
}
