#include "gtest/gtest.h"
#include "PIDCtrl.h"

using namespace xero::misc ;

TEST(PIDCtrl, BasicTest)
{
    PIDCtrl ctrl ;
    ctrl.init(1.0, 0.0, 0.0, 0.0, 0.0) ;

    double v = ctrl.getOutput(100.0, 98.0, 0.0, 0.1) ;
    EXPECT_DOUBLE_EQ(v, 2.0) ;
}

