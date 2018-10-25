#include "SorterInputModel.h"
#include <RobotSimBase.h>

using namespace frc ;

namespace xero {
    namespace sim {
        namespace bunny2018 {
            SorterInputModel::SorterInputModel(RobotSimBase &simbase) : SubsystemModel(simbase, "intake") {
                motor_channel_ = simbase.getSettingsParser().getInteger("hw:sorter:input:motor") ;
                voltage_ = 0.0 ;
            }

            SorterInputModel::~SorterInputModel() {                
            }

            std::string SorterInputModel::toString() {
                std::string result("bunnycollector: ") ;

                result += "duty_cycle " + std::to_string(voltage_) ;

                return result ;
            }

            void SorterInputModel::run(double dt) {                
            }

	        void SorterInputModel::inputChanged(SimulatedObject *obj) {              
			    std::lock_guard<std::mutex> lock(getLockMutex()) ;
                VictorSP *victor = dynamic_cast<VictorSP *>(obj) ;
                if (victor != nullptr) {
    			    if (victor == motor_)
                        voltage_ = victor->Get() ;
                }
            }    

            void SorterInputModel::addVictorSP(frc::VictorSP *motor) {
                if (motor->GetChannel() == motor_channel_) {
                    motor_ = motor ;
                    motor_->addModel(this) ;
                }
            }        
        }
    }
}
