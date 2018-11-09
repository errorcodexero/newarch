#include "BunnyCollectorModel.h"
#include <TalonSRX.h>
#include <RobotSimBase.h>

using namespace frc ;
using namespace ctre::phoenix::motorcontrol::can ;

namespace xero {
    namespace sim {
        namespace bunny2018 {
            BunnyCollectorModel::BunnyCollectorModel(RobotSimBase &simbase) : SubsystemModel(simbase, "intake") {
                motor_channel_ = simbase.getSettingsParser().getInteger("hw:bunnycollector:motor") ;
                voltage_ = 0.0 ;
            }

            BunnyCollectorModel::~BunnyCollectorModel() {                
            }

            std::string BunnyCollectorModel::toString() {
                std::string result("bunnycollector: ") ;

                result += "duty_cycle " + std::to_string(voltage_) ;

                return result ;
            }

            void BunnyCollectorModel::run(double dt) {                
            }

	        void BunnyCollectorModel::inputChanged(SimulatedObject *obj) {              
			    std::lock_guard<std::mutex> lock(getLockMutex()) ;
                TalonSRX *talon = dynamic_cast<TalonSRX *>(obj) ;
                if (talon != nullptr) {
    			    if (talon == motor_)
                        voltage_ = talon->Get() ;
                }
            }    

            void BunnyCollectorModel::addTalonSRX(TalonSRX *motor) {
                if (motor->GetDeviceID() == motor_channel_) {
                    motor_ = motor ;
                    motor_->addModel(this) ;
                }
            }        
        }
    }
}
