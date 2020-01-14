#include <catch.h>
#include "TrapezoidalProfile.h"

using namespace xero::misc ;

TEST_CASE("Trapezoidal Speed Profile")
{
    SECTION("Positive V Test")
    {
        TrapezoidalProfile profile(2, -4, 100) ;

        profile.update(96.0, 0.0, 0.0);
        REQUIRE_THAT(profile.getTimeAccel(), Catch::Matchers::Floating::WithinAbsMatcher(8.0, 1e-6));
        REQUIRE_THAT(profile.getTimeCruise(), Catch::Matchers::Floating::WithinAbsMatcher(0.0, 1e-6));
        REQUIRE_THAT(profile.getTimeDecel(), Catch::Matchers::Floating::WithinAbsMatcher(4.0, 1e-6));
        REQUIRE_THAT(profile.getActualMaxVelocity(), Catch::Matchers::Floating::WithinAbsMatcher(16.0, 1e-6));
        REQUIRE_THAT(profile.getDistance(4.0), Catch::Matchers::Floating::WithinAbsMatcher(16.0, 1e-6));
        REQUIRE_THAT(profile.getVelocity(4.0), Catch::Matchers::Floating::WithinAbsMatcher(8.0, 1e-6));
        REQUIRE_THAT(profile.getDistance(10.0), Catch::Matchers::Floating::WithinAbsMatcher(88.0, 1e-6));
        REQUIRE_THAT(profile.getVelocity(10.0), Catch::Matchers::Floating::WithinAbsMatcher(8.0, 1e-6));
    }

    SECTION("Negative V Test")
    {
        TrapezoidalProfile profile(2, -4, 100) ;

        profile.update(-96.0, 0.0, 0.0) ;
        REQUIRE_THAT(profile.getTimeAccel(), Catch::Matchers::Floating::WithinAbsMatcher(8.0, 1e-6));
        REQUIRE_THAT(profile.getTimeCruise(), Catch::Matchers::Floating::WithinAbsMatcher(0.0, 1e-6));
        REQUIRE_THAT(profile.getTimeDecel(), Catch::Matchers::Floating::WithinAbsMatcher(4.0, 1e-6));
        REQUIRE_THAT(profile.getActualMaxVelocity(), Catch::Matchers::Floating::WithinAbsMatcher(-16.0, 1e-6));
        REQUIRE_THAT(profile.getDistance(4.0), Catch::Matchers::Floating::WithinAbsMatcher(-16.0, 1e-6));
        REQUIRE_THAT(profile.getVelocity(4.0), Catch::Matchers::Floating::WithinAbsMatcher(-8.0, 1e-6));
        REQUIRE_THAT(profile.getDistance(10.0), Catch::Matchers::Floating::WithinAbsMatcher(-88.0, 1e-6));
        REQUIRE_THAT(profile.getVelocity(10.0), Catch::Matchers::Floating::WithinAbsMatcher(-8.0, 1e-6));
    }

    SECTION("Posivitve Non Zero Start End V Test")
    {
        TrapezoidalProfile profile(2, -6, 100) ;

        profile.update(309.0, 12.0, 22.0) ;
        REQUIRE_THAT(profile.getTimeAccel(), Catch::Matchers::Floating::WithinAbsMatcher(11.0, 1e-6));
        REQUIRE_THAT(profile.getTimeCruise(), Catch::Matchers::Floating::WithinAbsMatcher(0.0, 1e-6));
        REQUIRE_THAT(profile.getTimeDecel(), Catch::Matchers::Floating::WithinAbsMatcher(2.0, 1e-6));
        REQUIRE_THAT(profile.getActualMaxVelocity(), Catch::Matchers::Floating::WithinAbsMatcher(34.0, 1e-6));
        REQUIRE_THAT(profile.getDistance(9.0), Catch::Matchers::Floating::WithinAbsMatcher(189.0, 1e-6));
        REQUIRE_THAT(profile.getVelocity(9.0), Catch::Matchers::Floating::WithinAbsMatcher(30.0, 1e-6));
        REQUIRE_THAT(profile.getDistance(12.0), Catch::Matchers::Floating::WithinAbsMatcher(284.0, 1e-6));
        REQUIRE_THAT(profile.getVelocity(12.0), Catch::Matchers::Floating::WithinAbsMatcher(28.0, 1e-6));
    }

    SECTION("Negative Non Zero Start End V Test")
    {
        TrapezoidalProfile profile(2, -6, 100) ;

        profile.update(-309.0, -12.0, -22.0) ;
        REQUIRE_THAT(profile.getTimeAccel(), Catch::Matchers::Floating::WithinAbsMatcher(11.0, 1e-6));
        REQUIRE_THAT(profile.getTimeCruise(), Catch::Matchers::Floating::WithinAbsMatcher(0.0, 1e-6));
        REQUIRE_THAT(profile.getTimeDecel(), Catch::Matchers::Floating::WithinAbsMatcher(2.0, 1e-6));
        REQUIRE_THAT(profile.getActualMaxVelocity(), Catch::Matchers::Floating::WithinAbsMatcher(-34.0, 1e-6));
        REQUIRE_THAT(profile.getDistance(9.0), Catch::Matchers::Floating::WithinAbsMatcher(-189.0, 1e-6));
        REQUIRE_THAT(profile.getVelocity(9.0), Catch::Matchers::Floating::WithinAbsMatcher(-30.0, 1e-6));
        REQUIRE_THAT(profile.getDistance(12.0), Catch::Matchers::Floating::WithinAbsMatcher(-284.0, 1e-6));
        REQUIRE_THAT(profile.getVelocity(12.0), Catch::Matchers::Floating::WithinAbsMatcher(-28.0, 1e-6));
    }

    SECTION("Deceleration Only")
    {
        TrapezoidalProfile profile(2, -4, 40) ;

        profile.update(200, 40.0, 0.0) ;
        REQUIRE_THAT(profile.getTimeAccel(), Catch::Matchers::Floating::WithinAbsMatcher(0.0, 1e-6));
        REQUIRE_THAT(profile.getTimeCruise(), Catch::Matchers::Floating::WithinAbsMatcher(0.0, 1e-6));
        REQUIRE_THAT(profile.getTimeDecel(), Catch::Matchers::Floating::WithinAbsMatcher(10.0, 1e-6));
        REQUIRE_THAT(profile.getActualMaxVelocity(), Catch::Matchers::Floating::WithinAbsMatcher(40.0, 1e-6));
        REQUIRE_THAT(profile.getDistance(4.0), Catch::Matchers::Floating::WithinAbsMatcher(128.0, 1e-6));
        REQUIRE_THAT(profile.getVelocity(4.0), Catch::Matchers::Floating::WithinAbsMatcher(24.0, 1e-6));
        REQUIRE_THAT(profile.getDistance(7.0), Catch::Matchers::Floating::WithinAbsMatcher(182.0, 1e-6));
        REQUIRE_THAT(profile.getVelocity(7.0), Catch::Matchers::Floating::WithinAbsMatcher(12.0, 1e-6));        
    }

    SECTION("Max Velocity Cruize Plus Deceleration")
    {
        TrapezoidalProfile profile(2, -8, 40) ;

        profile.update(500, 40.0, 0.0) ;
        REQUIRE_THAT(profile.getTimeAccel(), Catch::Matchers::Floating::WithinAbsMatcher(0.0, 1e-6));
        REQUIRE_THAT(profile.getTimeCruise(), Catch::Matchers::Floating::WithinAbsMatcher(10.0, 1e-6));
        REQUIRE_THAT(profile.getTimeDecel(), Catch::Matchers::Floating::WithinAbsMatcher(5.0, 1e-6));
        REQUIRE_THAT(profile.getActualMaxVelocity(), Catch::Matchers::Floating::WithinAbsMatcher(40.0, 1e-6));
        REQUIRE_THAT(profile.getDistance(5.0), Catch::Matchers::Floating::WithinAbsMatcher(200.0, 1e-6));
        REQUIRE_THAT(profile.getVelocity(5.0), Catch::Matchers::Floating::WithinAbsMatcher(40.0, 1e-6));
        REQUIRE_THAT(profile.getDistance(13.0), Catch::Matchers::Floating::WithinAbsMatcher(484.0, 1e-6));
        REQUIRE_THAT(profile.getVelocity(13.0), Catch::Matchers::Floating::WithinAbsMatcher(16.0, 1e-6));  
    }

    SECTION("Trapezoid")
    {
        TrapezoidalProfile profile(2, -4, 20) ;

        profile.update(550, 0.0, 0.0) ;
        REQUIRE_THAT(profile.getTimeAccel(), Catch::Matchers::Floating::WithinAbsMatcher(10.0, 1e-6));
        REQUIRE_THAT(profile.getTimeCruise(), Catch::Matchers::Floating::WithinAbsMatcher(20.0, 1e-6));
        REQUIRE_THAT(profile.getTimeDecel(), Catch::Matchers::Floating::WithinAbsMatcher(5.0, 1e-6));
        REQUIRE_THAT(profile.getActualMaxVelocity(), Catch::Matchers::Floating::WithinAbsMatcher(20.0, 1e-6));
        REQUIRE_THAT(profile.getDistance(5.0), Catch::Matchers::Floating::WithinAbsMatcher(25.0, 1e-6));
        REQUIRE_THAT(profile.getVelocity(5.0), Catch::Matchers::Floating::WithinAbsMatcher(10.0, 1e-6));
        REQUIRE_THAT(profile.getDistance(20.0), Catch::Matchers::Floating::WithinAbsMatcher(300.0, 1e-6));
        REQUIRE_THAT(profile.getVelocity(20.0), Catch::Matchers::Floating::WithinAbsMatcher(20.0, 1e-6));  
        REQUIRE_THAT(profile.getDistance(33.0), Catch::Matchers::Floating::WithinAbsMatcher(542.0, 1e-6));
        REQUIRE_THAT(profile.getVelocity(33.0), Catch::Matchers::Floating::WithinAbsMatcher(8.0, 1e-6));          
    }

    SECTION("Complex Trapezoid")
    {
        TrapezoidalProfile profile(9, -13, 95) ;

        profile.update(2564, 5.0, 69.0) ;
        REQUIRE_THAT(profile.getTimeAccel(), Catch::Matchers::Floating::WithinAbsMatcher(10.0, 1e-6));
        REQUIRE_THAT(profile.getTimeCruise(), Catch::Matchers::Floating::WithinAbsMatcher(20.0, 1e-6));
        REQUIRE_THAT(profile.getTimeDecel(), Catch::Matchers::Floating::WithinAbsMatcher(2.0, 1e-6));
        REQUIRE_THAT(profile.getActualMaxVelocity(), Catch::Matchers::Floating::WithinAbsMatcher(95.0, 1e-6));
    }

    SECTION("Start Velocity > Max Velocity") {
        TrapezoidalProfile profile(2, -1, 2);
        profile.update(/*distance*/100, /*startVel*/5, /*endVel*/0);
        REQUIRE_THAT(profile.getTimeAccel(), Catch::Matchers::Floating::WithinAbsMatcher(3, 1e-6));
        REQUIRE_THAT(profile.getAccel(0), Catch::Matchers::Floating::WithinAbsMatcher(-1, 1e-6));
        
        REQUIRE_THAT(profile.getVelocity(0), Catch::Matchers::Floating::WithinAbsMatcher(5, 1e-6));
        REQUIRE_THAT(profile.getVelocity(1), Catch::Matchers::Floating::WithinAbsMatcher(4, 1e-6));
        REQUIRE_THAT(profile.getVelocity(2), Catch::Matchers::Floating::WithinAbsMatcher(3, 1e-6));
        REQUIRE_THAT(profile.getVelocity(3), Catch::Matchers::Floating::WithinAbsMatcher(2, 1e-6));
        
        REQUIRE_THAT(profile.getAccel(3), Catch::Matchers::Floating::WithinAbsMatcher(0, 1e-6));
        REQUIRE_THAT(profile.getDistance(3), Catch::Matchers::Floating::WithinAbsMatcher(10.5, 1e-6));

        REQUIRE_THAT(profile.getTimeDecel(), Catch::Matchers::Floating::WithinAbsMatcher(2, 1e-6));
        // accel dist = 10.5
        // decel dist = 2
        // cruise dist = 87.5

        REQUIRE_THAT(profile.getTimeCruise(), Catch::Matchers::Floating::WithinAbsMatcher(43.75, 1e-6));
        REQUIRE_THAT(profile.getTotalTime(), Catch::Matchers::Floating::WithinAbsMatcher(3 + 43.75 + 2, 1e-6));
        REQUIRE_THAT(profile.getDistance(3 + 43.75), Catch::Matchers::Floating::WithinAbsMatcher(10.5 + 87.5, 1e-6));

        REQUIRE_THAT(profile.getDistance(3 + 43.75 + 2), Catch::Matchers::Floating::WithinAbsMatcher(100, 1e-6));
    }
}