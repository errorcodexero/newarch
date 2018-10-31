#include "SorterModel.h"
#include <RobotSimBase.h>
#include <xeromath.h>

using namespace frc ;

namespace xero {
    namespace sim {
        namespace bunny2018 {
            SorterModel::SorterModel(RobotSimBase &simbase) : SubsystemModel(simbase, "intake") {
                motor_channel_ = simbase.getSettingsParser().getInteger("hw:sorter:motor") ;
				encoder_channel_1_ = simbase.getSettingsParser().getInteger("hw:sorter:encoder_1") ;
				encoder_channel_2_ = simbase.getSettingsParser().getInteger("hw:sorter:encoder_2") ;
				index_sensor_ = simbase.getSettingsParser().getInteger("hw:sorter:index") ;
				ticks_per_revolution_ = simbase.getSettingsParser().getInteger("aorter:ticks_per_revolution") ;
                voltage_ = 0.0 ;
				angle_ = 0.0 ;
            }

            SorterModel::~SorterModel() {
            }

            std::string SorterModel::toString() {
                std::string result("bunnycollector: ") ;

                result += "duty_cycle " + std::to_string(voltage_) ;

                return result ;
            }

            void SorterModel::run(double dt) {                
            }

	        void SorterModel::inputChanged(SimulatedObject *obj) {
			    std::lock_guard<std::mutex> lock(getLockMutex()) ;
                VictorSP *victor = dynamic_cast<VictorSP *>(obj) ;
                if (victor != nullptr) {
    			    if (victor == motor_)
                        voltage_ = victor->Get() ;
                }
            }    

            void SorterModel::addVictorSP(frc::VictorSP *motor) {
                if (motor->GetChannel() == motor_channel_) {
                    motor_ = motor ;
                    motor_->addModel(this) ;
                }
            }

	        void SorterModel::addEncoder(frc::Encoder *encoder) {
                int first, second ;

                encoder->SimulatorGetDigitalIOs(first, second) ;
                if (first == encoder_channel_1_ && second == encoder_channel_2_ || first == encoder_channel_2_ && second == encoder_channel_1_) {
                    encoder_ = encoder ;
                    encoder_->addModel(this) ;
                    encoder_->SimulatorSetValue(getEncoderValue()) ;
                }
            }			

            void SorterModel::addDigitalInput(frc::DigitalInput *input) {
                if (input->GetChannel() == index_sensor_) {
                    index_ = input ;
                    index_->SimulatorSetValue(false) ;
                    index_->addModel(this) ;
                }
            }

			int SorterModel::getEncoderValue() {
				return static_cast<int>(angle_ / 2.0 / xero::math::PI * ticks_per_revolution_) ;
			}
        }
    }
}
