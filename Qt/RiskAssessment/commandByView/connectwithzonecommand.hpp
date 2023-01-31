#ifndef CONNECTWITHZONECOMMAND_HPP
#define CONNECTWITHZONECOMMAND_HPP

#include "core/command.hpp"

class DiagramItem;
class RiskEditController;

class ConnectWithZoneCommand : public Command
{
    ConnectCommandDescriptor descriptor;
public:
    ConnectWithZoneCommand(ConnectCommandDescriptor descriptor);

    void execute() override;
};

#endif // CONNECTWITHZONECOMMAND_HPP
