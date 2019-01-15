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

void MyRobot::Autonomous()
{
    double duration = 10.0 ;
    double start = frc::Timer::GetFPGATimestamp() ;
    std::cout << "Auto Start time: " << start << std::endl ;
    std::cout << "Start Encoders: " << left_enc_p->Get() << ", " << right_enc_p->Get() << std::endl ;

    left_p->Set(ControlMode::PercentOutput, 1) ;
    right_p->Set(ControlMode::PercentOutput, 1) ;

    while (IsAutonomous() && frc::Timer::GetFPGATimestamp() - start < duration) ;
    std::cout << "End time " << frc::Timer::GetFPGATimestamp() << std::endl ;
    std::cout << "End Encoders: " << left_enc_p->Get() << ", " << right_enc_p->Get() << std::endl ;

    left_p->Set(ControlMode::PercentOutput, 0) ;
    right_p->Set(ControlMode::PercentOutput, 0) ;   

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
