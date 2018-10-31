#include "BunnyCollectorModel.h"
#include <RobotSimBase.h>

using namespace frc ;

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
                VictorSP *victor = dynamic_cast<VictorSP *>(obj) ;
                if (victor != nullptr) {
    			    if (victor == motor_)
                        voltage_ = victor->Get() ;
                }
            }    

            void BunnyCollectorModel::addVictorSP(frc::VictorSP *motor) {
                if (motor->GetChannel() == motor_channel_) {
                    motor_ = motor ;
                    motor_->addModel(this) ;
                }
            }        
        }
    }
}
