#ifndef ROLE_H
#define ROLE_H

#include <algorithm>

#include "model.hpp"
#include "operation.hpp"

class LPriv;

class Role : public Model {
    Q_OBJECT
private:
    std::vector<std::weak_ptr<LPriv>> lPrivs;

    void removeLPriv(std::vector<std::weak_ptr<LPriv>>::iterator iterator);
protected:
    /*virtual QString getNextID() override {
        return QString::number(idCounter++);
    }*/

    static int idCount;
public:

    explicit Role(QObject* parent = nullptr);
    Role(QString name, QObject* parent = nullptr);

    void addLPriv(std::weak_ptr<LPriv> priv);

    class GrantLPrivToRoleVisitor: public ModelVisitor {
    private:
        Role& role;
    public:
        GrantLPrivToRoleVisitor(Role& role): role(role){}

        void visits(Operation& op){
            role.addLPriv(op.getLPriv());
        }
    };

    std::unique_ptr<ModelVisitor> getGrantLPrivVisitor() override {
        return std::make_unique<GrantLPrivToRoleVisitor>(*this);
    }

    void visitedBy(ModelVisitor& visitor) override {
        visitor.visits(*this);
    }

    void removeLPriv(std::weak_ptr<LPriv> priv);

    void forEachLPriv(std::function<void(std::weak_ptr<LPriv>)> lambda){
        std::vector<std::weak_ptr<LPriv>> lprivsCopy = lPrivs;
        for(auto& lprivWeak:lprivsCopy){
            lambda(lprivWeak);
        }
    }

    QJsonObject serialize() const override;
    void loadConnections(QJsonObject jsonObj, std::unordered_map<QString, Serializable *> addressTable) override;

    //signal emitter methods
    void removeLPrivForOperation(QString opName);
};

#endif // ROLE_H
