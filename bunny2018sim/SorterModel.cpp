#include "SorterModel.h"
#include <RobotSimBase.h>
#include <xeromath.h>

using namespace frc ;
using namespace ctre::phoenix::motorcontrol::can ;

namespace xero {
    namespace sim {
        namespace bunny2018 {
            SorterModel::SorterModel(RobotSimBase &simbase) : SubsystemModel(simbase, "sorter") {
                sorter_motor_channel_ = simbase.getSettingsParser().getInteger("hw:sorter:motor") ;
                in_motor_channel_ = simbase.getSettingsParser().getInteger("hw:sorter:in:motor") ;
                out_motor_channel_ = simbase.getSettingsParser().getInteger("hw:sorter:out:motor") ;								
				encoder_channel_1_ = simbase.getSettingsParser().getInteger("hw:sorter:encoder_1") ;
				encoder_channel_2_ = simbase.getSettingsParser().getInteger("hw:sorter:encoder_2") ;
				index_sensor_ = simbase.getSettingsParser().getInteger("hw:sorter:index") ;
				ticks_per_revolution_ = simbase.getSettingsParser().getInteger("sorter:ticks_per_revolution") ;
				int holes = simbase.getSettingsParser().getInteger("sorter:total_holes") ;
				degrees_per_hole_ = 2 * xero::math::PI / holes ;
                sorter_voltage_ = 0.0 ;
                in_voltage_ = 0.0 ;
                out_voltage_ = 0.0 ;								
				angle_ = 0.0 ;
            }

            SorterModel::~SorterModel() {
            }

            std::string SorterModel::toString() {
                std::string result("sorter ") ;

                return result ;
            }

			/// \brief run the sorter model
			/// The model assumes that the zero degree point of rotation is exactly between
			/// two of the holes in a solid area.
            void SorterModel::run(double dt) {
            }

	        void SorterModel::inputChanged(SimulatedObject *obj) {
			    std::lock_guard<std::mutex> lock(getLockMutex()) ;
                TalonSRX *talon = dynamic_cast<TalonSRX *>(obj) ;
                if (talon != nullptr) {
    			    if (talon == sortermotor_)
                        sorter_voltage_ = talon->Get() ;
					else if (talon == inmotor_)
						in_voltage_ = talon->Get() ;
					else if (talon == outmotor_)
						out_voltage_ = talon->Get() ;
                }
            }    

            void SorterModel::addTalonSRX(TalonSRX *motor) {
                if (motor->GetDeviceID() == sorter_motor_channel_) {
                    sortermotor_ = motor ;
                    sortermotor_->addModel(this) ;
                }
				else if (motor->GetDeviceID() == in_motor_channel_) {
                    inmotor_ = motor ;
                    inmotor_->addModel(this) ;
                }
				else if (motor->GetDeviceID() == out_motor_channel_) {
                    outmotor_ = motor ;
                    outmotor_->addModel(this) ;
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
