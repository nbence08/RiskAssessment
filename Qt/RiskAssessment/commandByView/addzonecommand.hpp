#ifndef ADDZONECOMMAND_HPP
#define ADDZONECOMMAND_HPP

#include "core/command.hpp"
#include <QString>

class RiskEditController;

struct AddZoneCommandDescriptor{
    float x, y;
    RiskEditController& controller;
    QString name;
};

template <typename MODEL, typename VIEW>
class AddZoneCommand : public Command {
    AddZoneCommandDescriptor descriptor;
public:
    AddZoneCommand(AddZoneCommandDescriptor descriptor):descriptor(descriptor) {}

    void execute() override;
};

#endif // ADDZONECOMMAND_HPP
