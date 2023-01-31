#ifndef ACTOR_H
#define ACTOR_H

#include <vector>
#include <algorithm>

#include "model.hpp"
#include "role.hpp"

class Connection;
class LPriv;
class PAccess;
class Path;

class Actor : public Model {
private:    
    std::vector<std::weak_ptr<Connection>> connections;
    std::vector<std::weak_ptr<LPriv>> lPrivs;
    std::vector<std::weak_ptr<PAccess>> pAccesses;
    std::vector<Role*> roles;

    class ActorGrantPAccessVisitor : public ModelVisitor {
    public:
        ActorGrantPAccessVisitor(Model& model);

        void visits(PhysicalZone& pzone) override;
        void visits(Device& device) override;
    };

    class ActorGrantLPrivVisitor : public ModelVisitor {
    public:
        ActorGrantLPrivVisitor(Model& model);

        void visits(Role& role) override;
        void visits(Operation& operation) override;
        void visits(Service& service) override;
    };

    class ActorGrantConnectionVisitor : public ModelVisitor {
    public:
        ActorGrantConnectionVisitor(Model& model);

        void visits(Service& service) override;
        void visits(Operation& operation) override;
    };

public:
    Actor(QObject* parent = nullptr);
    Actor(QString name, QObject* parent = nullptr);

    virtual std::unique_ptr<ModelVisitor> getGrantPAccessVisitor() override;
    virtual std::unique_ptr<ModelVisitor> getGrantLPrivVisitor() override;
    virtual std::unique_ptr<ModelVisitor> getGrantConnectionVisitor() override;

    void visitedBy(ModelVisitor& visitor) override {
        visitor.visits(*this);
    }

    void addLPriv(std::weak_ptr<LPriv> lpriv);

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

    QJsonObject serialize() const override;
    void loadConnections(QJsonObject jsonObj, std::unordered_map<QString, Serializable*> addressTable) override;

    //signal emitter methods
    void removeLPrivForOperation(QString opName);
    void removeLPrivLike(QString roleOrLPrivName);
    void removeConnectionTo(QString serviceName);
    void removePAccessTo(QString paccName);

    //possible implementation in the future, if defined
    /*virtual std::unique_ptr<RiskVisitor> getConnectToZoneVisitor() override {
        return std::make_unique<ActorConnectToZoneVisitor>(this);
    }*/

};

#endif // ACTOR_H
