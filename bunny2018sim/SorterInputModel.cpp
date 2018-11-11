#include "SorterInputModel.h"
#include <RobotSimBase.h>

using namespace frc ;
using namespace ctre::phoenix::motorcontrol::can ;

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
                std::string result("sorterinput ") ;

                result += "duty_cycle " + std::to_string(voltage_) ;

                return result ;
            }

            void SorterInputModel::run(double dt) {                
            }

	        void SorterInputModel::inputChanged(SimulatedObject *obj) {              
			    std::lock_guard<std::mutex> lock(getLockMutex()) ;
                TalonSRX *talon = dynamic_cast<TalonSRX *>(obj) ;
                if (talon != nullptr) {
    			    if (talon == motor_)
                        voltage_ = talon->Get() ;
                }
            }    

            void SorterInputModel::addTalonSRX(TalonSRX *motor) {
                if (motor->GetDeviceID() == motor_channel_) {
                    motor_ = motor ;
                    motor_->addModel(this) ;
                }
            }        
        }
    }
}
