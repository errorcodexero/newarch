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
                xero::sim2::SimulatorEngine &engine = xero::sim2::SimulatorEngine::getEngine();
                auto mgr = engine.getCTREManager();
                mgr->destroy(index_);                
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

            int IMotorController::GetSelectedSensorPosition() {
                double ret;

                xero::sim2::SimulatorEngine &engine = xero::sim2::SimulatorEngine::getEngine();
                auto mgr = engine.getCTREManager();

                mgr->getEncoder(index_, ret) ;

                return static_cast<int>(ret);
            }

            void IMotorController::SetSelectedSensorPosition(int pos) {
                xero::sim2::SimulatorEngine &engine = xero::sim2::SimulatorEngine::getEngine();
                auto mgr = engine.getCTREManager();

                mgr->setEncoder(index_, pos) ;
            }            

        } // namespace motorcontrol
    } // namespace phoenix
} // namespace ctre
