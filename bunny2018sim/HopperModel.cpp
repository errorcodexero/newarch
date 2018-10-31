#include "HopperModel.h"
#include <RobotSimBase.h>

using namespace frc ;

namespace xero {
    namespace sim {
        namespace bunny2018 {
            HopperModel::HopperModel(RobotSimBase &simbase) : SubsystemModel(simbase, "intake") {
                motor_channel_ = simbase.getSettingsParser().getInteger("hw:hopper:motor") ;
                voltage_ = 0.0 ;
            }

            HopperModel::~HopperModel() {                
            }

            std::string HopperModel::toString() {
                std::string result("bunnycollector: ") ;

                result += "duty_cycle " + std::to_string(voltage_) ;

                return result ;
            }

            void HopperModel::run(double dt) {                
            }

	        void HopperModel::inputChanged(SimulatedObject *obj) {              
			    std::lock_guard<std::mutex> lock(getLockMutex()) ;
                VictorSP *victor = dynamic_cast<VictorSP *>(obj) ;
                if (victor != nullptr) {
    			    if (victor == motor_)
                        voltage_ = victor->Get() ;
                }
            }    

            void HopperModel::addVictorSP(frc::VictorSP *motor) {
                if (motor->GetChannel() == motor_channel_) {
                    motor_ = motor ;
                    motor_->addModel(this) ;
                }
            }        
        }
    }
}
