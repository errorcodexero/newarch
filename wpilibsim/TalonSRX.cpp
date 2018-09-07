#include "TalonSRX.h"
#include "RobotSimBase.h"

using namespace xero::sim;

namespace ctre
{
namespace phoenix
{
namespace motorcontrol
{
namespace can
{

TalonSRX::TalonSRX(int index)
{
	index_ = index;

	RobotSimBase &sim = RobotSimBase::getRobotSimulator();
	sim.connect(this);
}

TalonSRX::~TalonSRX()
{
	RobotSimBase &sim = RobotSimBase::getRobotSimulator();
	sim.disconnect(this);
}

void TalonSRX::Set(ControlMode mode, double v)
{
	value_ = v;
	changed() ;
}

SensorCollection& TalonSRX::GetSensorCollection() {
	return sensors_ ;
}

} // namespace can
} // namespace motorcontrol
} // namespace phoenix
} // namespace ctre
