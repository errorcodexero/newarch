#include "HopperModel.h"
#include <frc/RobotSimBase.h>

using namespace frc ;
using namespace ctre::phoenix::motorcontrol::can ;

namespace xero {
    namespace sim {
        namespace bunny2018 {
            HopperModel::HopperModel(RobotSimBase &simbase) : SubsystemModel(simbase, "hopper") {
                motor_channel_ = simbase.getSettingsParser().getInteger("hw:hopper:motor") ;
                voltage_ = 0.0 ;
            }

            HopperModel::~HopperModel() {                
            }

            std::string HopperModel::toString() {
                std::string result("hopper: ") ;

                result += "duty_cycle " + std::to_string(voltage_) ;

                return result ;
            }

            void HopperModel::run(double dt) {                
            }

            void HopperModel::inputChanged(SimulatedObject *obj) {              
                std::lock_guard<std::mutex> lock(getLockMutex()) ;
                TalonSRX *talon = dynamic_cast<TalonSRX *>(obj) ;
                if (talon != nullptr) {
                    if (talon == motor_)
                        voltage_ = talon->Get() ;
                }
            }    

            void HopperModel::addTalonSRX(TalonSRX *motor) {
                if (motor->GetDeviceID() == motor_channel_) {
                    motor_ = motor ;
                    motor_->addModel(this) ;
                }
            }        
        }
    }
}
