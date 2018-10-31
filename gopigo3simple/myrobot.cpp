#include "myrobot.h"
#include <Timer.h>

void MyRobot::RobotInit()
{
	left_p = new frc::GPG3MotorController(0) ;
	right_p = new frc::GPG3MotorController(1) ;
}

void MyRobot::Disabled()
{
}

void MyRobot::Autonomous()
{
	double duration = 3 ;
	double start = frc::Timer::GetFPGATimestamp() ;
	left_p->Set(1) ;
	right_p->Set(1) ;

	while (frc::Timer::GetFPGATimestamp() - start < duration) ;
	left_p->Set(1) ;
	right_p->Set(1) ;	
}

void MyRobot::OperatorControl()
{	
}

void MyRobot::Test()
{	
}

START_ROBOT_CLASS(MyRobot)
