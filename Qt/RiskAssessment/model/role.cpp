#include "role.hpp"
#include "lpriv.hpp"

void Role::removeLPriv(std::vector<std::weak_ptr<LPriv>>::iterator iterator){
    lPrivs.erase(iterator);
}

Role::Role(QObject *parent)
    : Model{"RL", parent}
{}

Role::Role(QString name, QObject* parent): Model(name, "RL", parent) {  }

void Role::addLPriv(std::weak_ptr<LPriv> priv){
    lPrivs.push_back(priv);
    emit this->lPrivGranted(this, &(priv.lock()->getOperation() ));
}

QJsonObject Role::serialize() const {
    QJsonObject jsonRole;
    jsonRole["name"] = objectName();
    jsonRole["type"] = "Role";

    QJsonArray lprivsArr;
    for(auto& lpriv:lPrivs){
        lprivsArr.push_back(lpriv.lock()->getOperation().objectName());
    }
    jsonRole[ROLE_CONTAINS_LPRIV] = lprivsArr;
    return jsonRole;
}

void Role::loadConnections(QJsonObject jsonObj, std::unordered_map<QString, Serializable *> addressTable){
    QJsonArray opNames = jsonObj[ROLE_CONTAINS_LPRIV].toArray();

    for(auto name:opNames){
        Operation* op = static_cast<Operation*>(addressTable.at(name.toString()));
        requestGrantLPrivVisit(*op);
    }
}

void Role::removeLPrivForOperation(QString opName){
    auto privIt = lPrivs.begin();
    for(;privIt != lPrivs.end(); ++privIt){
        auto& privOp = privIt->lock()->getOperation();
        auto privOpName = privOp.objectName();

        if(privOpName.compare(opName) == 0){
            lPrivs.erase(privIt);
            emit this->lprivDeleted(this, &privOp);
            return;
        }
    }
}

void Role::removeLPriv(std::weak_ptr<LPriv> privToLookForWeak){
    auto privToLookForPtr = privToLookForWeak.lock();
    for(auto it = lPrivs.begin(); it != lPrivs.end(); ++it){
        auto privFound = it->lock();
        if(privFound == privToLookForPtr){
            std::weak_ptr<LPriv> priv = *it;

            lPrivs.erase(it);
            emit this->lprivDeleted(this, &(priv.lock()->getOperation()));

            return;
        }
    }
    throw std::out_of_range("LPriv is not within lprivs, therefore cannot be removed");
}

int Role::idCount = 0;
