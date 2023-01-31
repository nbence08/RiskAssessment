#ifndef ADDITEMCOMMAND_HPP
#define ADDITEMCOMMAND_HPP

#include "core/command.hpp"
#include "riskeditcontroller.hpp"

struct AddItemCommandDescriptor{
    float x, y;
    RiskEditController& riskEditController;

};

template <typename MODEL, typename VIEW>
class AddItemCommand : public Command
{
    AddItemCommandDescriptor descriptor;
    std::pair<MODEL*, VIEW*> createdPair;
public:
    AddItemCommand(AddItemCommandDescriptor addItemDescriptor):descriptor(addItemDescriptor){}
    void execute() override;
};

template<typename MODEL, typename VIEW>
void AddItemCommand<MODEL, VIEW>::execute() {
    auto mv = descriptor.riskEditController.createMVPair<MODEL, VIEW>();
    auto view = mv.second;

    view->setPos(QPointF(descriptor.x, descriptor.y));
}


#endif // ADDITEMCOMMAND_HPP
