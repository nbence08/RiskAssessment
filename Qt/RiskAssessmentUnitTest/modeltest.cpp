#include "modeltest.hpp"
#include "allmodels.hpp"

using namespace testing;

TEST(ModelTest, InitMultipleNamesTest){
    Operation operation("OPERAT");
    Service service("SERVICE");
    PhysicalZone pzone("PZ");

    EXPECT_EQ(operation.objectName(), "OP_0_OPERAT");
    EXPECT_EQ(service.objectName(), "SV_2_SERVICE");
    EXPECT_EQ(pzone.objectName(), "PZ_4_PZ");

    Model::resetCounter();
}

TEST(ModelTest, ResetModelCounterTest){
    Operation operation("OPERAT");
    Model::resetCounter();
    Service service("SERVICE");
    Model::resetCounter();
    PhysicalZone pzone("PZ");
    Model::resetCounter();

    EXPECT_EQ(operation.objectName(), "OP_0_OPERAT");
    EXPECT_EQ(service.objectName(), "SV_0_SERVICE");
    EXPECT_EQ(pzone.objectName(), "PZ_0_PZ");
}

TEST(ModelTest, VisitorTest){
    MockVisitor visitor;

    Actor actor;
    Device device;
    Employee emp;
    LogicalZone lzone;
    Operation op;
    Path path;
    PhysicalZone pzone;
    Role role;
    Service service;
    ServiceClass svClass;

    PAccess& pacc = *(pzone.getPAccess().lock());
    LPriv& priv = *(op.getLPriv().lock());
    Connection& conn = *(service.getConnection().lock());

    EXPECT_CALL(visitor, visitsActor()).Times(1);
    EXPECT_CALL(visitor, visitsDevice()).Times(1);
    EXPECT_CALL(visitor, visitsEmployee()).Times(1);
    EXPECT_CALL(visitor, visitsLogicalZone()).Times(1);
    EXPECT_CALL(visitor, visitsOperation()).Times(1);
    EXPECT_CALL(visitor, visitsPath()).Times(1);
    EXPECT_CALL(visitor, visitsPhysicalZone()).Times(1);
    EXPECT_CALL(visitor, visitsRole()).Times(1);
    EXPECT_CALL(visitor, visitsService()).Times(1);
    EXPECT_CALL(visitor, visitsServiceClass()).Times(1);
    EXPECT_CALL(visitor, visitsPAccess()).Times(1);
    EXPECT_CALL(visitor, visitsLPriv()).Times(1);
    EXPECT_CALL(visitor, visitsConnection()).Times(1);

    actor.visitedBy(visitor);
    device.visitedBy(visitor);
    emp.visitedBy(visitor);
    lzone.visitedBy(visitor);
    op.visitedBy(visitor);
    path.visitedBy(visitor);
    pzone.visitedBy(visitor);
    role.visitedBy(visitor);
    service.visitedBy(visitor);
    svClass.visitedBy(visitor);
    pacc.visitedBy(visitor);
    priv.visitedBy(visitor);
    conn.visitedBy(visitor);

    Model::resetCounter();
}

