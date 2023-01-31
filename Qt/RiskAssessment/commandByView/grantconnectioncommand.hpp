#ifndef GRANTCONNECTIONCOMMAND_HPP
#define GRANTCONNECTIONCOMMAND_HPP

#include "core/command.hpp"

class GrantConnectionCommand : public Command
{
    ConnectCommandDescriptor descriptor;
public:
    GrantConnectionCommand(ConnectCommandDescriptor descriptor): descriptor(descriptor) {}

    void execute() override;
};

#endif // GRANTCONNECTIONCOMMAND_HPP
