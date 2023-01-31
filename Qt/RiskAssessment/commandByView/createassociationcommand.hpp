#ifndef CREATEASSOCIATIONCOMMAND_HPP
#define CREATEASSOCIATIONCOMMAND_HPP

#include "core/command.hpp"

class CreateAssociationCommand : public Command
{
    ConnectCommandDescriptor descriptor;
public:
    CreateAssociationCommand(ConnectCommandDescriptor descriptor): descriptor(descriptor) {}

    void execute() override;
};

#endif // CREATEASSOCIATIONCOMMAND_HPP
