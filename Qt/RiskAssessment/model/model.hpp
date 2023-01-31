#ifndef MODEL_H
#define MODEL_H
#pragma once

#include <memory>
#include "serializable.hpp"

#include "visitor.hpp"

class PAccess;
class Connection;
class LPriv;
struct AssociationData;

class Model : public QObject, public Serializable {
    Q_OBJECT
private:
    inline static int idCounter = 0;
public:
    static void resetCounter(){
        idCounter = 0;
    }
protected:
    QString idNum;
    QString namePrefix;

    virtual QString getNameType(){
        return "MODEL";
    }

    virtual void getNextID(){
        idNum = QString::number(idCounter++);
    }

    void modelGetNextID(){
        getNextID();
    }

public:
    Model(QString namePrefix = "MO", QObject* parent = nullptr);
    Model(QString name, QString namePrefix = "MO", QObject* parent = nullptr);

    virtual ~Model();

    void makeName(QString newName = "");

    virtual void requestGrantPAccessVisit(Model& model);
    virtual void requestGrantLPrivVisit(Model& model);
    virtual void requestGrantConnectionVisit(Model& model);
    virtual void requestConnectToZoneVisit(Model& model);
    virtual void requestCreateAssociationToModel(Model& model);

    virtual std::unique_ptr<ModelVisitor> getGrantPAccessVisitor(){
        return std::make_unique<ModelVisitor>();
    }
    virtual std::unique_ptr<ModelVisitor> getGrantLPrivVisitor(){
        return std::make_unique<ModelVisitor>();
    }
    virtual std::unique_ptr<ModelVisitor> getGrantConnectionVisitor(){
        return std::make_unique<ModelVisitor>();
    }
    virtual std::unique_ptr<ModelVisitor> getConnectToZoneVisitor(){
        return std::make_unique<ModelVisitor>();
    }
    virtual std::unique_ptr<ModelVisitor> getCreateAssociationToVisitor(){
        return std::make_unique<ModelVisitor>();
    }

    virtual void visitedBy(ModelVisitor& /*visitor*/) {}

signals:
    void pAccessGranted(Model* from, Model* to);
    void lPrivGranted(Model* from, Model* lPriv);
    void connectionAddedFromTo(Model* from, Model* to);
    void itemConnectedToZone(Model* item, Model* zone);
    void modelAssociatedToModel(AssociationData data);

    //void itemDisconnectedFromZone(Model* item, Model* zone);

    void lprivDeleted(Model* model, Model* lpriv);
    void connectionDeleted(Model* model, Model* connection);
    void pAccessDeleted(Model* model, Model* pacc);
    void itemDisconnectedFromZone(Model* model, Model* zone);
    void associationDeleted(Model* model, Model* assoc);

    void modelDeleted(Model* model);
};

struct AssociationData {
    Model* from;
    Model* to;
    QString name = "Association";
};

#endif // MODEL_H
