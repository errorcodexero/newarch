#include "SorterOutputModel.h"
#include <RobotSimBase.h>

using namespace frc ;
using namespace ctre::phoenix::motorcontrol::can ;

namespace xero {
    namespace sim {
        namespace bunny2018 {
            SorterOutputModel::SorterOutputModel(RobotSimBase &simbase) : SubsystemModel(simbase, "intake") {
                motor_channel_ = simbase.getSettingsParser().getInteger("hw:sorter:output:motor") ;
                voltage_ = 0.0 ;
            }

            SorterOutputModel::~SorterOutputModel() {                
            }

            std::string SorterOutputModel::toString() {
                std::string result("sorteroutput ") ;

                result += "duty_cycle " + std::to_string(voltage_) ;

                return result ;
            }

            void SorterOutputModel::run(double dt) {                
            }

	        void SorterOutputModel::inputChanged(SimulatedObject *obj) {              
			    std::lock_guard<std::mutex> lock(getLockMutex()) ;
                TalonSRX *talon = dynamic_cast<TalonSRX *>(obj) ;
                if (talon != nullptr) {
    			    if (talon == motor_)
                        voltage_ = talon->Get() ;
                }
            }    

            void SorterOutputModel::addTalonSRX(TalonSRX *motor) {
                if (motor->GetDeviceID() == motor_channel_) {
                    motor_ = motor ;
                    motor_->addModel(this) ;
                }
            }        
        }
    }
}
