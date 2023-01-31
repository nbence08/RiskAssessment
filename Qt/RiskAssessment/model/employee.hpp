#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <vector>

#include "model.hpp"
#include "role.hpp"

class Connection;
class LPriv;
class PAccess;

class Employee : public Model {
private:
    std::vector<std::weak_ptr<Connection>> connections;
    std::vector<std::weak_ptr<LPriv>> lPrivs;
    std::vector<std::weak_ptr<PAccess>> pAccesses;
    std::vector<Role*> roles;

    class EmployeeGrantPAccessVisitor : public ModelVisitor {
    public:
        EmployeeGrantPAccessVisitor(Model& model): ModelVisitor(&model) {}

        void visits(PhysicalZone& pzone) override;
        void visits(Device& device) override;
    };

    class EmployeeGrantLPrivVisitor : public ModelVisitor {
    public:
        EmployeeGrantLPrivVisitor(Model& model): ModelVisitor(&model) {}

        void visits(Role& role) override;
        void visits(Operation& operation) override;
        void visits(Service& service) override;
    };

    class EmployeeGrantConnectionVisitor : public ModelVisitor {
    public:
        EmployeeGrantConnectionVisitor(Model& model): ModelVisitor(&model) {}

        void visits(Service& service) override;
        void visits(Operation& operation) override;
    };
protected:
    /*virtual QString getNextID() override {
        return QString::number(idCounter++);
    }*/
public:
    Employee(QObject* parent = nullptr);
    Employee(QString name, QObject* parent = nullptr);

    QJsonObject serialize() const override;
    void loadConnections(QJsonObject jsonObj, std::unordered_map<QString, Serializable*> addressTable) override;

    virtual std::unique_ptr<ModelVisitor> getGrantPAccessVisitor() override {
        return std::make_unique<EmployeeGrantPAccessVisitor>(*this);
    }
    virtual std::unique_ptr<ModelVisitor> getGrantLPrivVisitor() override {
        return std::make_unique<EmployeeGrantLPrivVisitor>(*this);
    }
    virtual std::unique_ptr<ModelVisitor> getGrantConnectionVisitor() override {
        return std::make_unique<EmployeeGrantConnectionVisitor>(*this);
    }

    void visitedBy(ModelVisitor& visitor) override {
        visitor.visits(*this);
    }

    void addLPriv(std::weak_ptr<LPriv> lpriv){
        lPrivs.push_back(lpriv);
    }

    void foreachLPriv(std::function<void(LPriv&)> lambda){
        std::vector<std::weak_ptr<LPriv>> privsCopy = lPrivs;
        for(auto& lpriv:privsCopy){
            lambda(*lpriv.lock());
        }
        for(auto role:roles){
            role->forEachLPriv([&lambda=lambda](std::weak_ptr<LPriv> lpWeak){
               lambda(*lpWeak.lock());
            });
        }
    }

    void foreachOwnedLPriv(std::function<void(LPriv&)> lambda){
        std::vector<std::weak_ptr<LPriv>> privsCopy = lPrivs;
        for(auto& lpriv:privsCopy){
            lambda(*lpriv.lock());
        }
    }

    void foreachRole(std::function<void(Role&)> lambda){
        std::vector<Role*> rolesCopy = roles;
        for(auto& role:rolesCopy){
            lambda(*role);
        }
    }

    //signal emitter methods
    void removeLPrivForOperation(QString opName);
    void removeLPrivLike(QString roleOrLPrivName);
    void removeConnectionTo(QString serviceName);
    void removePAccessTo(QString paccName);
};

#endif // EMPLOYEE_H
