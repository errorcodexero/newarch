#include "IntakeModel.h"

using namespace frc ;

namespace xero {
    namespace sim {
        namespace phoenix {
            IntakeModel::IntakeModel() {
                voltage1_ = 0.0 ;
                voltage2_ = 0.0 ;
            }

            IntakeModel::~IntakeModel() {                
            }

            std::string IntakeModel::toString() {
                std::string result("intake: ") ;

                result += "duty_cycle_1 " + std::to_string(voltage1_) ;
                result += ", duty_cycle_2 " + std::to_string(voltage2_) ;

                return result ;
            }

            void IntakeModel::run(double dt) {                
            }

	        void IntakeModel::inputChanged(SimulatedObject *obj) {              
			    std::lock_guard<std::mutex> lock(getLockMutex()) ;
                VictorSP *victor = dynamic_cast<VictorSP *>(obj) ;
                if (victor != nullptr) {
    			    if (victor == motor1_)
                        voltage1_ = victor->Get() ;
			        else if (victor == motor2_)
                        voltage2_ = victor->Get() ;
                }
            }    

            void IntakeModel::addVictorSP(frc::VictorSP *motor) {
                if (motor->GetChannel() == 3) {
                    motor1_ = motor ;
                    motor1_->addModel(this) ;
                }
                else if (motor->GetChannel() == 4) {
                    motor2_ = motor ;
                    motor2_->addModel(this) ;
                }
            }        
        }
    }
}
