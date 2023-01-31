#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <stdexcept>
#include <functional>

class DiagramItem;
class RiskEditController;

struct ConnectCommandDescriptor{
    DiagramItem& from;
    DiagramItem& to;
    RiskEditController& controller;
};

class Command
{
protected:
    bool executed;
public:
    Command() {}
    virtual void execute() {
        throw std::logic_error("unimplemented method");
    };
    virtual void revert() {
        throw std::logic_error("unimplemented method");
    }
};

class LambdaCommand : public Command{

    std::function<void()> executeLambda;
    std::function<void()> revertLambda;
public:
    LambdaCommand(std::function<void()> execute, std::function<void()> revert): executeLambda(execute), revertLambda(revert) { }

    virtual void execute() override {
        executeLambda();
    }

    virtual void revert() override {
        revertLambda();
    }
};

#endif // COMMAND_HPP
