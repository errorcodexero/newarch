#include "AHRS.h"
#include "RobotSimulator.h"

AHRS::AHRS(const char *port_p)
{
}

AHRS::~AHRS()
{
}

double AHRS::GetYaw()
{
	frc::RobotSimulator &robot = frc::RobotSimulator::get();
	return robot.getYaw();
}
void AHRS::ZeroYaw()
{
	frc::RobotSimulator &robot = frc::RobotSimulator::get();
	return robot.zeroYaw();
}
