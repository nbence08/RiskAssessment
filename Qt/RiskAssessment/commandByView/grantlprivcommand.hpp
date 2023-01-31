#ifndef GRANTLPRIVCOMMAND_HPP
#define GRANTLPRIVCOMMAND_HPP

#include "core/command.hpp"

class GrantLPrivCommand : public Command
{
    ConnectCommandDescriptor descriptor;
public:
    GrantLPrivCommand(ConnectCommandDescriptor descriptor): descriptor(descriptor) {}

    void execute() override;
};

#endif // GRANTLPRIVCOMMAND_HPP
