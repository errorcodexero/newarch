#include "IMotorController.h"
#include <frc/RobotSimBase.h>

using namespace xero::sim;

namespace ctre
{
    namespace phoenix
    {
        namespace motorcontrol
        {
            IMotorController::IMotorController(int index)
            {
                index_ = index;
                inverted_ = false ;
            }

            IMotorController::~IMotorController()
            {
            }

            void IMotorController::Set(ControlMode mode, double v)
            {
                if (inverted_)
                    value_ = -v;
                else
                    value_ = v ;
                    
                changed() ;
            }

            SensorCollection& IMotorController::GetSensorCollection() {
                return sensors_ ;
            }

        } // namespace motorcontrol
    } // namespace phoenix
} // namespace ctre
