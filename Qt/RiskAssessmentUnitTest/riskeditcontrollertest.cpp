#include "riskeditcontrollertest.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <gmock/gmock-matchers.h>

#include "riskeditcontroller.hpp"
#include "allmodels.hpp"
#include "allviews.hpp"

using namespace testing;

class MockGraphicsView: public DiagramItem{
public:
    MOCK_METHOD1(modelDeleted, void(Model* model));
};

TEST_F(RiskAssessmentControllerTest, CreateServicePairTest){
    auto [svModel,svView] = controller.createMVPair<Service, ServiceGraphicsView>("ABC");
    EXPECT_EQ(svModel->objectName(), "SV_0_ABC");
    EXPECT_EQ(svView->objectName(), "SV_0_ABC_VIEW");
    EXPECT_EQ(svView->getModel(), svModel);

    Model::resetCounter();
}

TEST_F(RiskAssessmentControllerTest, CreateDevicePairTest){

    auto [dvModel, dvView] = controller.createMVPair<Device, DeviceGraphicsView>("DEVV");
    EXPECT_EQ(dvModel->objectName(), "DV_0_DEVV");
    EXPECT_EQ(dvView->objectName(), "DV_0_DEVV_VIEW");
    EXPECT_EQ(dvView->getModel(), dvModel);

    Model::resetCounter();
}

