#ifndef VISITOR_HPP
#define VISITOR_HPP

#pragma once
#include <QObject>
#include <unordered_map>

class Model;
class Actor;
class Connection;
class Device;
class Employee;
class LogicalZone;
class LPriv;
class Operation;
class PAccess;
class Path;
class PhysicalZone;
class Resource;
class Role;
class Service;
class ServiceClass;

class ModelVisitor{
protected:
    virtual void defaultOperation(Model& model);
    Model* model;

public:
    ModelVisitor(Model* model);
    ModelVisitor();
    void visits(Model* model);
    virtual void visits(Actor& actor);
    virtual void visits(Connection& connection);
    virtual void visits(Device& device);
    virtual void visits(Employee& employee);
    virtual void visits(LogicalZone& lzone);
    virtual void visits(LPriv& lpriv);
    virtual void visits(Operation& operation);
    virtual void visits(PAccess& paccess);
    virtual void visits(Path& path);
    virtual void visits(PhysicalZone& pzone);
    virtual void visits(Role& role);
    virtual void visits(Service& service);
    virtual void visits(ServiceClass& sc);
};

class SearchModelIDVisitor : public ModelVisitor {
private:
    QString wantedName;
    Model* model;

    void setModelIfIDEqualsToWantedName(Model& model);
public:
    virtual void visits(Actor& actor) override;
    virtual void visits(Connection& connection) override;
    virtual void visits(Device& device) override;
    virtual void visits(Employee& employee) override;
    virtual void visits(LogicalZone& lzone) override;
    virtual void visits(LPriv& lpriv) override;
    virtual void visits(Operation& operation) override;
    virtual void visits(PAccess& paccess) override;
    virtual void visits(PhysicalZone& pzone) override;
    virtual void visits(Role& role) override;
    virtual void visits(Service& service) override;
    virtual void visits(ServiceClass& sc) override;

    Model& getModel();
};

class FindModelsByIDVisitor: public ModelVisitor {
private:
    std::vector<QString> modelsToFind;
    std::unordered_map<QString, Model*> modelsFound;

    virtual void defaultOperation(Model& model) override;
public:
    FindModelsByIDVisitor(std::vector<QString> modelsToFind);

    Model* operator[](QString& name);
};

/*
 * actor
    // slot overrides
    void onLprivDeleted(Model* lpriv) override;
    void onConnectionDeleted(Model* to) override;
    void onPAccessDeleted(Model* to) override;

    employee
    // slot overrides
    void onLprivDeleted(Model* lpriv) override;
    void onConnectionDeleted(Model* to) override;
    void onPAccessDeleted(Model* to) override;

    operation
    virtual void onConnectionDeleted(Model* connection) override;
    virtual void onPAccessDeleted(Model* pacc) override;
    virtual void onAssociationDeleted(Model* assoc) override;

    role
    void onLprivDeleted(Model* lpriv) override;

    service
    void onAssociationDeleted(Model* assoc) override;
*/
namespace Visitor{


class RemoveLPrivByOperationNameVisitor : public ModelVisitor{
    QString name;
    bool removeRole;

    template <typename ENTITY>
    void removeLPriv(ENTITY& entity){
        if(!removeRole){
            entity.removeLPrivForOperation(name);
        }
        else {
            entity.removeLPrivLike(name);
        }
    }
public:
    RemoveLPrivByOperationNameVisitor(QString name, bool removeRole=false):name(name),removeRole(removeRole){}
    void visits(Actor& actor);
    void visits(Employee& emp);
    void visits(Role& role);
    void visits(Operation& op);
};

class RemovePAccessByPzoneNameVisitor : public ModelVisitor{
    QString name;
public:
    RemovePAccessByPzoneNameVisitor(QString name):name(name){}
    void visits(Actor& actor);
    void visits(Employee& emp);
    void visits(Operation& op);
};

class RemoveConnectionByServiceNameVisitor : public ModelVisitor{
    QString name;
public:
    RemoveConnectionByServiceNameVisitor(QString name):name(name){}
    void visits(Actor& actor);
    void visits(Employee& emp);
    void visits(Operation& op);
};

class DeleteAssociationByNameVisitor : public ModelVisitor{
    QString name;
public:
    DeleteAssociationByNameVisitor(QString name):name(name){}
    void visits(Device& dev);
    void visits(Service& service);
    void visits(ServiceClass& serviceClass);
};

class DisconnectItemFromZoneByNameVisitor : public ModelVisitor{
    QString name;
public:
    DisconnectItemFromZoneByNameVisitor(QString name):name(name){}
    void visits(Device& dev);
    void visits(PhysicalZone& zone);
};

}
//Visitor implementation names should have the following format:
// [creator][user action]Visitor
// creator is the class of object, which creates the visitor, and stores itself in the visitor as model
// user action is associated with the input mode, the user has selected
// "Visitor" is always at the end of a visitor name
/*
e.g.:
class DeviceConnectWithZoneVisitor: public RiskVisitor{
    [implementation]
};
*/

#endif // VISITOR_HPP
