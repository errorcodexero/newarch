#include "myrobot.h"
#include <frc/Timer.h>
#include <iostream>

using namespace frc ;
using namespace ctre::phoenix::motorcontrol;
using namespace ctre::phoenix::motorcontrol::can ;

void MyRobot::RobotInit()
{
    left_enc_p = new Encoder(0, 1) ;    
    right_enc_p = new Encoder(2, 3) ;

    left_enc_p->Reset() ;
    right_enc_p->Reset() ;

    left_p = new TalonSRX(1) ;
    right_p = new TalonSRX(2) ;

    right_p->SetInverted(true) ;
}

void MyRobot::Disabled()
{
    std::cout << "Robot Disabled" << std::endl ;
}

void MyRobot::runMotors(int phase, double left, double right, double duration)
{
    double start ; 
    std::chrono::milliseconds ms20(20) ;

    start = frc::Timer::GetFPGATimestamp() ;
    std::cout << "Phase " << phase << ": start time: " << start << std::endl ;
    std::cout << "Phase " << phase << ": start encoders: " << left_enc_p->Get() ;
    std::cout << ", " << right_enc_p->Get() << std::endl ;
    std::cout << "Phase " << phase << ": left = " << left << " right = " << right << std::endl ;

    //
    // Phase one - run the right motor for 3 seconds, positive direction
    //
    left_p->Set(ControlMode::PercentOutput, left) ; 
    right_p->Set(ControlMode::PercentOutput, right) ;

    start = frc::Timer::GetFPGATimestamp() ;
    while (IsAutonomous() && frc::Timer::GetFPGATimestamp() - start < duration)
    {
        //
        // Sleeping for 20ms in this loop basically gives pthreads the ability
        // to go and run another this while we sleep.  This ensures the control thread
        // that moves the robot between states gets its fair share of time to run.
        //
        std::this_thread::sleep_for(ms20) ;
    }
    std::cout << "Phase " << phase << ": end time " << frc::Timer::GetFPGATimestamp() << std::endl ;
    std::cout << "Phase " << phase << ": end encoders: " << left_enc_p->Get()  ;
    std::cout << ", " << right_enc_p->Get() << std::endl ;
}

void MyRobot::Autonomous()
{


    runMotors(1, 0.0, 1.0, 2.0) ;
    runMotors(2, 0.0, 1.0, 2.0) ;
    runMotors(99, 0.0, 0.0, 2.0) ;

    std::cout << "Reset Encoders" << std::endl ;
    left_enc_p->Reset() ;
    right_enc_p->Reset() ;

    runMotors(3, 0.0, -1.0, 2.0) ;
    runMotors(4, 0.0, -1.0, 2.0) ;
    runMotors(99, 0.0, 0.0, 2.0) ;

    std::cout << "Reset Encoders" << std::endl ;
    left_enc_p->Reset() ;
    right_enc_p->Reset() ;

    runMotors(5, 1.0, 0.0, 2.0) ;
    runMotors(6, 1.0, 0.0, 2.0) ;
    runMotors(99, 0.0, 0.0, 2.0) ;

    std::cout << "Reset Encoders" << std::endl ;
    left_enc_p->Reset() ;
    right_enc_p->Reset() ;

    runMotors(7, -1.0, 0.0, 2.0) ;
    runMotors(8, -1.0, 0.0, 2.0) ;
    runMotors(99, 0.0, 0.0, 2.0) ;    

    std::cout << "Reset Encoders" << std::endl ;
    left_enc_p->Reset() ;
    right_enc_p->Reset() ;  

    runMotors(9, 0.0, 0.0, 1.0) ;    

    while (IsAutonomous()) ;
    std::cout << "Autonomous mode ending " << frc::Timer::GetFPGATimestamp() << std::endl ;
}

void MyRobot::OperatorControl()
{   
    std::cout << "Teleop Start time: " << frc::Timer::GetFPGATimestamp() << std::endl ;
    while (IsOperatorControl()) ;
    std::cout << "Teleop mode ending " << frc::Timer::GetFPGATimestamp() << std::endl ; 
}

void MyRobot::Test()
{   
}

START_ROBOT_CLASS(MyRobot)
