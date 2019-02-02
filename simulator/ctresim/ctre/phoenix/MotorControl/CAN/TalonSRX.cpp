#include "TalonSRX.h"
#include <frc/RobotSimBase.h>

using namespace xero::sim;

namespace ctre
{
namespace phoenix
{
namespace motorcontrol
{
namespace can
{

TalonSRX::TalonSRX(int index) : IMotorController(index)
{
    RobotSimBase &sim = RobotSimBase::getRobotSimulator();
    sim.connect(this);
}

TalonSRX::~TalonSRX()
{
    RobotSimBase &sim = RobotSimBase::getRobotSimulator();
    sim.disconnect(this);
}

} // namespace can
} // namespace motorcontrol
} // namespace phoenix
} // namespace ctre
