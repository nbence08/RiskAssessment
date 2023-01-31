#include "connection.hpp"

Connection::Connection(Service& service, QObject* parent):Model("CO", parent), service(service){}
