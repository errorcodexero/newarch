#include <catch.h>
#include "PIDCtrl.h"

using namespace xero::misc ;

TEST_CASE("PID Controller", "BasicTest")
{
    PIDCtrl ctrl ;
    ctrl.init(1.0, 0.0, 0.0, 0.0, 0.0) ;

    double v = ctrl.getOutput(100.0, 98.0, 0.1) ;
    REQUIRE_THAT(v, Catch::Matchers::Floating::WithinAbsMatcher(2.0, 0.01)) ;
}
