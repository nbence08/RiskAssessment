#include "connectwithzonecommand.hpp"

#include "riskeditcontroller.hpp"

ConnectWithZoneCommand::ConnectWithZoneCommand(ConnectCommandDescriptor descriptor):
    descriptor(descriptor)
{

}

void ConnectWithZoneCommand::execute() {
    descriptor.controller.connectItemToZone(descriptor.from.getModel(), descriptor.to.getModel());
}
