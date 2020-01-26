#include "ctre/phoenix/MotorControl/IMotorController.h"
#include <SimulatorEngine.h>
#include <CTREManager.h>

namespace ctre
{
    namespace phoenix
    {
        namespace motorcontrol
        {
            IMotorController::IMotorController(int index) : sensors_(index) 
            {
                index_ = index;
                inverted_ = false ;

                xero::sim2::SimulatorEngine &engine = xero::sim2::SimulatorEngine::getEngine();
                auto mgr = engine.getCTREManager();
                mgr->create(index);
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

                if (value_ > 1.0)
                    value_ = 1.0 ;
                else if (value_ < -1.0)
                    value_ = -1.0 ;

                xero::sim2::SimulatorEngine &engine = xero::sim2::SimulatorEngine::getEngine();
                auto mgr = engine.getCTREManager();
                mgr->set(index_, value_);
            }

            SensorCollection& IMotorController::GetSensorCollection() {
                return sensors_ ;
            }

        } // namespace motorcontrol
    } // namespace phoenix
} // namespace ctre
