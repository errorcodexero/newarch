#pragma once

#include <frc/SampleRobot.h>
#include <frc/Encoder.h>
#include <ctre/Phoenix.h>

class MyRobot: public frc::SampleRobot {
public:
    virtual void RobotInit();
    virtual void Disabled();
    virtual void Autonomous();
    virtual void OperatorControl();
    virtual void Test();
    
private:
    ctre::phoenix::motorcontrol::can::TalonSRX *left_p ;
    ctre::phoenix::motorcontrol::can::TalonSRX *right_p ;

    frc::Encoder *left_enc_p ;
    frc::Encoder *right_enc_p ;
} ;
