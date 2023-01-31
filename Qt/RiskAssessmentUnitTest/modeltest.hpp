#ifndef MODELTEST_HPP
#define MODELTEST_HPP

#include "visitor.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <gmock/gmock-matchers.h>

class MockVisitor: public ModelVisitor {
public:
    MOCK_METHOD(void, visitsEmployee, ());
    MOCK_METHOD(void, visitsActor, ());
    MOCK_METHOD(void, visitsDevice, ());
    MOCK_METHOD(void, visitsConnection, ());
    MOCK_METHOD(void, visitsLogicalZone, ());
    MOCK_METHOD(void, visitsLPriv, ());
    MOCK_METHOD(void, visitsOperation, ());
    MOCK_METHOD(void, visitsPAccess, ());
    MOCK_METHOD(void, visitsPath, ());
    MOCK_METHOD(void, visitsPhysicalZone, ());
    MOCK_METHOD(void, visitsRole, ());
    MOCK_METHOD(void, visitsService, ());
    MOCK_METHOD(void, visitsServiceClass, ());

    void visits(Actor& /*actor*/) override {
        visitsActor();
    }
    void visits(Connection& /*connection*/) override {
        visitsConnection();
    }
    void visits(Device& /*device*/) override {
        visitsDevice();
    }
    void visits(Employee& /*employee*/) override {
        visitsEmployee();
    }
    void visits(LogicalZone& /*lzone*/) override {
        visitsLogicalZone();
    }
    void visits(LPriv& /*lpriv*/) override {
        visitsLPriv();
    }
    void visits(Operation& /*operation*/) override {
        visitsOperation();
    }
    void visits(PAccess& /*paccess*/) override {
        visitsPAccess();
    }
    void visits(Path& /*path*/) override {
        visitsPath();
    }
    void visits(PhysicalZone& /*pzone*/) override {
        visitsPhysicalZone();
    }
    void visits(Role& /*role*/) override {
        visitsRole();
    }
    void visits(Service& /*service*/) override {
        visitsService();
    }
    void visits(ServiceClass& /*sc*/) override {
        visitsServiceClass();
    }
};

#endif // MODELTEST_HPP
