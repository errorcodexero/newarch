#pragma once

#include <frc/SampleRobot.h>
#include <units/units.h>

using namespace units;

namespace frc
{
    class TimedRobot : public SampleRobot
    {
    public:
        TimedRobot(second_t looptime) ;
        virtual ~TimedRobot() ;

        virtual void Disabled() ;
        virtual void Autonomous() ;
        virtual void OperatorControl() ;
        virtual void Test() ;

        virtual void DisabledInit() { }
        virtual void DisabledPeriodic() { }

        virtual void AutonomousInit() { } 
        virtual void AutonomousPeriodic() { }

        virtual void TeleopInit() { }
        virtual void TeleopPeriodic() { }

        virtual void TestInit() { }
        virtual void TestPeriodic() { }

    private:
        std::chrono::milliseconds looptime_ ;
    } ;
}