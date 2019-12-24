#include "EncoderMapper.h"
#include <catch.h>

using namespace xero::misc ;

TEST_CASE("EncoderMapper", "BasicTest")
{
    SECTION("Simple linear scale")
    {
        //
        //  Encoder    Physical
        //   5.0         100
        //   4.5          90
        //   4.0          80
        //   3.5          70
        //   3.0          60
        //   2.5          50
        //   2.0          40
        //   1.5          30
        //   1.0          20
        //   0.5          10
        //   0.0           0
        //
        EncoderMapper mapper(100.0, 0.0, 5.0, 0.0) ;
        mapper.calibrate(50.0, 2.5) ;
        REQUIRE_THAT(mapper.toRobot(0.01), Catch::Matchers::Floating::WithinAbsMatcher(0.2, 1e-6)) ;
        REQUIRE_THAT(mapper.toRobot(-0.01), Catch::Matchers::Floating::WithinAbsMatcher(0.0, 1e-6)) ;        
        REQUIRE_THAT(mapper.toRobot(2.5), Catch::Matchers::Floating::WithinAbsMatcher(50.0, 1e-6)) ;
        REQUIRE_THAT(mapper.toRobot(4.99), Catch::Matchers::Floating::WithinAbsMatcher(99.8, 1e-6)) ;

        REQUIRE_THAT(mapper.toEncoder(0.0), Catch::Matchers::Floating::WithinAbsMatcher(0.0, 1e-6)) ;
        REQUIRE_THAT(mapper.toEncoder(50.0), Catch::Matchers::Floating::WithinAbsMatcher(2.5, 1e-6)) ;
        REQUIRE_THAT(mapper.toEncoder(100.0), Catch::Matchers::Floating::WithinAbsMatcher(5.0, 1e-6)) ;
    }


    SECTION("Simple linear scale wrapped")
    {
        //
        //  Encoder    Physical
        //   5.0          40
        //   4.5          30
        //   4.0          20
        //   3.5          10
        //   3.0           0
        //   2.5          90
        //   2.0          80
        //   1.5          70
        //   1.0          60
        //   0.5          50
        //   0.0          40
        //        
        EncoderMapper mapper(100.0, 0.0, 5.0, 0.0) ;
        mapper.calibrate(0.0, 3.0) ;
        REQUIRE_THAT(mapper.toRobot(0.0), Catch::Matchers::Floating::WithinAbsMatcher(40.0, 1e-6)) ;
        REQUIRE_THAT(mapper.toRobot(-0.01), Catch::Matchers::Floating::WithinAbsMatcher(40.0, 1e-6)) ;        
        REQUIRE_THAT(mapper.toRobot(2.5), Catch::Matchers::Floating::WithinAbsMatcher(90.0, 1e-6)) ;
        REQUIRE_THAT(mapper.toRobot(2.0), Catch::Matchers::Floating::WithinAbsMatcher(80.0, 1e-6)) ;
        REQUIRE_THAT(mapper.toRobot(4.0), Catch::Matchers::Floating::WithinAbsMatcher(20.0, 1e-6)) ;            
        REQUIRE_THAT(mapper.toRobot(5.0), Catch::Matchers::Floating::WithinAbsMatcher(40.0, 1e-6)) ;
        REQUIRE_THAT(mapper.toRobot(5.01), Catch::Matchers::Floating::WithinAbsMatcher(40.0, 1e-6)) ;
        REQUIRE_THAT(mapper.toRobot(2.99), Catch::Matchers::Floating::WithinAbsMatcher(99.8, 1e-6)) ;
        REQUIRE_THAT(mapper.toRobot(3.01), Catch::Matchers::Floating::WithinAbsMatcher(0.2, 1e-6)) ;        
        
        REQUIRE_THAT(mapper.toEncoder(80.0), Catch::Matchers::Floating::WithinAbsMatcher(2.0, 1e-6)) ;
        REQUIRE_THAT(mapper.toEncoder(20.0), Catch::Matchers::Floating::WithinAbsMatcher(4.0, 1e-6)) ;
    }

    SECTION("Simple linear scale flipped")
    {
        //
        //  Encoder    Physical
        //   5.0           0
        //   4.5          10
        //   4.0          20
        //   3.5          30
        //   3.0          40
        //   2.5          50
        //   2.0          60
        //   1.5          70
        //   1.0          80
        //   0.5          90
        //   0.0         100
        //        
        EncoderMapper mapper(100.0, 0.0, 0.0, 5.0) ;
        mapper.calibrate(50.0, 2.5) ;
        REQUIRE_THAT(mapper.toRobot(0.0), Catch::Matchers::Floating::WithinAbsMatcher(100.0, 1e-6)) ;
        REQUIRE_THAT(mapper.toRobot(-0.01), Catch::Matchers::Floating::WithinAbsMatcher(100.0, 1e-6)) ;        
        REQUIRE_THAT(mapper.toRobot(2.5), Catch::Matchers::Floating::WithinAbsMatcher(50.0, 1e-6)) ;
        REQUIRE_THAT(mapper.toRobot(4.9999999999), Catch::Matchers::Floating::WithinAbsMatcher(0.0, 1e-6)) ;
        REQUIRE_THAT(mapper.toRobot(1.75), Catch::Matchers::Floating::WithinAbsMatcher(65.0, 1e-6)) ;
        REQUIRE_THAT(mapper.toRobot(3.25), Catch::Matchers::Floating::WithinAbsMatcher(35.0, 1e-6)) ;     

        REQUIRE_THAT(mapper.toEncoder(50.0), Catch::Matchers::Floating::WithinAbsMatcher(2.5, 1e-6)) ;
        REQUIRE_THAT(mapper.toEncoder(35.0), Catch::Matchers::Floating::WithinAbsMatcher(3.25, 1e-6)) ;                   
    }

    SECTION("Linear scale flipped and wrapped")
    {
        //
        //  Encoder    Physical
        //   5.0          40
        //   4.5          50
        //   4.0          60
        //   3.5          70
        //   3.0          80
        //   2.5          90
        //   2.0           0
        //   1.5          10
        //   1.0          20
        //   0.5          30
        //   0.0          40
        //   
        EncoderMapper mapper(100.0, 0.0, 0.0, 5.0) ;
        mapper.calibrate(90.0, 2.5) ;
        REQUIRE_THAT(mapper.toRobot(0.0), Catch::Matchers::Floating::WithinAbsMatcher(40.0, 1e-6)) ;
        REQUIRE_THAT(mapper.toRobot(-0.01), Catch::Matchers::Floating::WithinAbsMatcher(40.0, 1e-6)) ;        
        REQUIRE_THAT(mapper.toRobot(2.5), Catch::Matchers::Floating::WithinAbsMatcher(90.0, 1e-6)) ;
        REQUIRE_THAT(mapper.toRobot(2.0), Catch::Matchers::Floating::WithinAbsMatcher(0.0, 1e-6)) ;        
        REQUIRE_THAT(mapper.toRobot(1.25), Catch::Matchers::Floating::WithinAbsMatcher(15.0, 1e-6)) ;
        REQUIRE_THAT(mapper.toRobot(3.75), Catch::Matchers::Floating::WithinAbsMatcher(65.0, 1e-6)) ;        
        REQUIRE_THAT(mapper.toRobot(4.0), Catch::Matchers::Floating::WithinAbsMatcher(60.0, 1e-6)) ;   

        REQUIRE_THAT(mapper.toEncoder(20.0), Catch::Matchers::Floating::WithinAbsMatcher(1.0, 1e-6)) ;      
        REQUIRE_THAT(mapper.toEncoder(80.0), Catch::Matchers::Floating::WithinAbsMatcher(3.0, 1e-6)) ;
    }    
}
