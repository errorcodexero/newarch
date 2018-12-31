#include "CollectorModel.h"
#include <TalonSRX.h>
#include <RobotSimBase.h>

using namespace frc ;
using namespace ctre::phoenix::motorcontrol::can ;

namespace xero {
    namespace sim {
        namespace bunny2018 {
            CollectorModel::CollectorModel(RobotSimBase &simbase) : SubsystemModel(simbase, "collector") {
                motor_channel_ = simbase.getSettingsParser().getInteger("hw:collector:motor") ;
                voltage_ = 0.0 ;
            }

            CollectorModel::~CollectorModel() {                
            }

            std::string CollectorModel::toString() {
                std::string result("collector: ") ;

                result += "duty_cycle " + std::to_string(voltage_) ;

                return result ;
            }

            void CollectorModel::run(double dt) {                
            }

            void CollectorModel::inputChanged(SimulatedObject *obj) {              
                std::lock_guard<std::mutex> lock(getLockMutex()) ;
                TalonSRX *talon = dynamic_cast<TalonSRX *>(obj) ;
                if (talon != nullptr) {
                    if (talon == motor_)
                        voltage_ = talon->Get() ;
                }
            }    

            void CollectorModel::addTalonSRX(TalonSRX *motor) {
                if (motor->GetDeviceID() == motor_channel_) {
                    motor_ = motor ;
                    motor_->addModel(this) ;
                }
            }        
        }
    }
}
