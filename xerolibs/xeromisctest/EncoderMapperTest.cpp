#include "EncoderMapper.h"
#include <catch.h>

using namespace xero::misc ;

TEST_CASE("EncoderMapper", "BasicTest")
{
    SECTION("Simple linear scale")
    {
        EncoderMapper mapper(100.0, 0.0, 5.0, 0.0) ;
        mapper.calibrate(50.0, 2.5) ;
        REQUIRE_THAT(mapper.toRobot(0.0), Catch::Matchers::Floating::WithinAbsMatcher(0.0, 1e-6)) ;
        REQUIRE_THAT(mapper.toRobot(-0.01), Catch::Matchers::Floating::WithinAbsMatcher(0.0, 1e-6)) ;        
        REQUIRE_THAT(mapper.toRobot(2.5), Catch::Matchers::Floating::WithinAbsMatcher(50.0, 1e-6)) ;
        REQUIRE_THAT(mapper.toRobot(5.0), Catch::Matchers::Floating::WithinAbsMatcher(100.0, 1e-6)) ;
        REQUIRE_THAT(mapper.toRobot(5.01), Catch::Matchers::Floating::WithinAbsMatcher(100.0, 1e-6)) ;        
    }

    SECTION("Simple linear scale wrapped")
    {
        EncoderMapper mapper(100.0, 0.0, 5.0, 0.0) ;
        mapper.calibrate(0.0, 3.0) ;
        REQUIRE_THAT(mapper.toRobot(0.0), Catch::Matchers::Floating::WithinAbsMatcher(40.0, 1e-6)) ;
        REQUIRE_THAT(mapper.toRobot(-0.01), Catch::Matchers::Floating::WithinAbsMatcher(40.0, 1e-6)) ;        
        REQUIRE_THAT(mapper.toRobot(2.5), Catch::Matchers::Floating::WithinAbsMatcher(90.0, 1e-6)) ;
        REQUIRE_THAT(mapper.toRobot(5.0), Catch::Matchers::Floating::WithinAbsMatcher(40.0, 1e-6)) ;
        REQUIRE_THAT(mapper.toRobot(5.01), Catch::Matchers::Floating::WithinAbsMatcher(40.0, 1e-6)) ;        
    }

    SECTION("Simple linear scale flipped")
    {
        EncoderMapper mapper(100.0, 0.0, 0.0, 5.0) ;
        mapper.calibrate(50.0, 2.5) ;
        REQUIRE_THAT(mapper.toRobot(0.0), Catch::Matchers::Floating::WithinAbsMatcher(100.0, 1e-6)) ;
        REQUIRE_THAT(mapper.toRobot(-0.01), Catch::Matchers::Floating::WithinAbsMatcher(100.0, 1e-6)) ;        
        REQUIRE_THAT(mapper.toRobot(2.5), Catch::Matchers::Floating::WithinAbsMatcher(50.0, 1e-6)) ;
        REQUIRE_THAT(mapper.toRobot(4.9999999999), Catch::Matchers::Floating::WithinAbsMatcher(0.0, 1e-6)) ;
        REQUIRE_THAT(mapper.toRobot(1.75), Catch::Matchers::Floating::WithinAbsMatcher(65.0, 1e-6)) ;
        REQUIRE_THAT(mapper.toRobot(3.25), Catch::Matchers::Floating::WithinAbsMatcher(35.0, 1e-6)) ;                
    }

    SECTION("Linear scale flipped and wrapped")
    {
        EncoderMapper mapper(100.0, 0.0, 0.0, 5.0) ;
        mapper.calibrate(90.0, 2.5) ;
        REQUIRE_THAT(mapper.toRobot(0.0), Catch::Matchers::Floating::WithinAbsMatcher(40.0, 1e-6)) ;
        REQUIRE_THAT(mapper.toRobot(-0.01), Catch::Matchers::Floating::WithinAbsMatcher(40.0, 1e-6)) ;        
        REQUIRE_THAT(mapper.toRobot(2.5), Catch::Matchers::Floating::WithinAbsMatcher(90.0, 1e-6)) ;
        REQUIRE_THAT(mapper.toRobot(1.25), Catch::Matchers::Floating::WithinAbsMatcher(15.0, 1e-6)) ;
        REQUIRE_THAT(mapper.toRobot(3.75), Catch::Matchers::Floating::WithinAbsMatcher(65.0, 1e-6)) ;        
    }    
}
