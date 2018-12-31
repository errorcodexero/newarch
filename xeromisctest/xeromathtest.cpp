#include "gtest/gtest.h"
#include "xeromath.h"

using namespace xero::misc ;

TEST(XeroMath, AngleDifference)
{
    double res ;

    EXPECT_DOUBLE_EQ(179, angleDifference(180.0, 1.0)) ;
    EXPECT_DOUBLE_EQ(2, angleDifference(1.0, -1.0)) ;
    EXPECT_DOUBLE_EQ(-2, angleDifference(179.0, -179.0)) ;
    EXPECT_DOUBLE_EQ(2, angleDifference(-179.0, 179.0)) ;
    EXPECT_DOUBLE_EQ(110, angleDifference(-160.0, 90.0)) ;
    EXPECT_DOUBLE_EQ(45, angleDifference(90.0, 45.0)) ;                    
}

