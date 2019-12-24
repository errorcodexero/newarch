#include <frc/TimedRobot.h>

namespace frc
{
    TimedRobot::TimedRobot(double looptime)
    {
        looptime_ = std::chrono::milliseconds(static_cast<int>(looptime * 1000)) ;
    }

    TimedRobot::~TimedRobot()
    {
    }

    void TimedRobot::Disabled()
    {
        DisabledInit() ;
        while (IsDisabled())
        {
            auto start = std::chrono::high_resolution_clock::now() ;
            DisabledPeriodic() ;

            auto elapsed = std::chrono::high_resolution_clock::now() - start ;
            auto elapsedms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed) ;
            auto remaining = looptime_ - elapsedms ;
            std::this_thread::sleep_for(remaining) ;
        }
    }

    void TimedRobot::Autonomous()
    {
        AutonomousInit() ;
        while (IsEnabled() && IsAutonomous())
        {
            auto start = std::chrono::high_resolution_clock::now() ;
            AutonomousPeriodic() ;

            auto elapsed = std::chrono::high_resolution_clock::now() - start ;
            auto elapsedms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed) ;
            auto remaining = looptime_ - elapsedms ;
            std::this_thread::sleep_for(remaining) ;
        }
    }

    void TimedRobot::OperatorControl()
    {
        TeleopInit() ;
        while (IsEnabled() && IsOperatorControl())
        {
            auto start = std::chrono::high_resolution_clock::now() ;
            TeleopPeriodic() ;

            auto elapsed = std::chrono::high_resolution_clock::now() - start ;
            auto elapsedms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed) ;
            auto remaining = looptime_ - elapsedms ;
            std::this_thread::sleep_for(remaining) ;
        }
    }

    void TimedRobot::Test()
    {
        TestInit() ;
        while (IsEnabled() && IsTest())
        {
            auto start = std::chrono::high_resolution_clock::now() ;
            TestPeriodic() ;

            auto elapsed = std::chrono::high_resolution_clock::now() - start ;
            auto elapsedms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed) ;
            auto remaining = looptime_ - elapsedms ;
            std::this_thread::sleep_for(remaining) ;
        }
    }
}