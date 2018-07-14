#include "AHRS.h"
#include "RobotSimulator.h"

AHRS::AHRS(frc::SPI::Port port)
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
