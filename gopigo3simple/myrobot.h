#pragma once

#include <SampleRobot.h>
#include <GPG3MotorController.h>

class MyRobot: public frc::SampleRobot {
public:
	virtual void RobotInit();
	virtual void Disabled();
	virtual void Autonomous();
	virtual void OperatorControl();
	virtual void Test();
	
private:
	frc::GPG3MotorController *left_p ;
	frc::GPG3MotorController *right_p ;
} ;
