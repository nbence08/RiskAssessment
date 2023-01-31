#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include <unordered_map>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

// identity
#define NAME "name"
#define TYPE "type"

//view
#define COORD_X "x"
#define COORD_Y "y"
#define COORD_Z "z"

#define HAS_CONNECTION "HasConnection"
#define HAS_LPRIV "HasLPriv"
#define HAS_PACCESS "HasPAccess"

#define OPERATION_CONTAINS_CONN "OperationContainsConnection"
#define OPERATION_CONTAINS_PACC "OperationContainsPAccess"
#define OPERATION_CONTAINS_LPRIV "OperationContainsLPriv"

#define PZONE_HAS_NEIGHBOR "PZoneHasNeighbor"
#define PZONE_CONTAINS_DEVICE "PZoneContainsDevice"
#define DEVICE_HAS_SERVICE "DeviceHasService"
#define SERVICE_CONTAINS_OPERATION "ServiceContainsOperation"
#define SERVICECLASS_CONTAINS_SERVICE "ServiceClassContainsService"

// helpers
#define ROLE_CONTAINS_LPRIV "RoleContainsLPriv"
#define LZONE_CONTAINS_DEVICE "LZoneContainsDevice"
#define HAS_ROLE "HasRole"

/*
#define ACTORS_ARR_NAME "actors"
#define DEVICES_ARR_NAME "devices"
#define EMPLOYEES_ARR_NAME "employees"
#define LZONES_ARR_NAME "lzones"
#define OPERATIONS_ARR_NAME "operations"
#define PZONES_ARR_NAME "pzones"
#define SERVICES_ARR_NAME "services"
*/

#define ACTOR_NAME "actor"
#define DEVICE_NAME "device"
#define EMPLOYEE_NAME "employee"
#define LZONE_NAME "lzone"
#define OPERATION_NAME "operation"
#define PZONE_NAME "pzone"
#define ROLE_NAME "role"
#define SERVICE_NAME "service"
#define LPRIV_NAME "lpriv"
#define CONNECTION_NAME "connection"
#define PACCESS_NAME "paccess"

// types
#define SERVICECLASS_TYPENAME "ServiceClass"

// connections are pointing to services
// lprivs are pointing to operations
// paccesses are pointing to pzones
#define OP_OWNED_CONNECTIONS "ownedConnections"
#define OP_REQUIRED_CONNECTION "requiredConnections"

class Serializable{
public:
    virtual QJsonObject serialize() const { return QJsonObject{}; };
    virtual void loadConnections(QJsonObject /*jsonObj*/, std::unordered_map<QString, Serializable*> /*addressTable*/) {};
};

#endif // SERIALIZABLE_H
