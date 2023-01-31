#ifndef GRANTPACCESSCOMMAND_HPP
#define GRANTPACCESSCOMMAND_HPP

#include "core/command.hpp"

class GrantPAccessCommand : public Command
{
    ConnectCommandDescriptor descriptor;
public:
    GrantPAccessCommand(ConnectCommandDescriptor descriptor): descriptor(descriptor) {}

    void execute() override;
};

#endif // GRANTPACCESSCOMMAND_HPP
