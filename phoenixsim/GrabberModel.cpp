#include "GrabberModel.h"
#include <RobotSimBase.h>

using namespace frc ;

namespace xero {
    namespace sim {
        namespace phoenix {
            GrabberModel::GrabberModel(RobotSimBase &simbase) : SubsystemModel(simbase, "grabber") {
                angle_ = 0.0 ;
                degrees_per_second_ = simbase.getSettingsParser().getDouble("grabber:sim:degrees_per_second") ;
                degrees_per_tick_ = simbase.getSettingsParser().getDouble("grabber:sim:degrees_per_tick") ;
                cube_angle_ = simbase.getSettingsParser().getDouble("grabber:sim:cube_angle") ;
                encoder_base_ = 0 ;

                motor_channel_ = simbase.getSettingsParser().getInteger("hw:grabber:motor") ;
                encoder_input_1_ = simbase.getSettingsParser().getInteger("hw:grabber:encoder1") ;
                encoder_input_2_ = simbase.getSettingsParser().getInteger("hw:grabber:encoder2") ;

                enc_ = nullptr ;
                voltage_ = 0.0 ;
            }

            GrabberModel::~GrabberModel() {
            }

			void GrabberModel::init() {
                if (getSimulator().hasProperty("grabber")) {
                    const std::string &prop = getSimulator().getProperty("grabber") ;
					double value ;
					if (parseDouble(prop, value)) {
						angle_ = value ;
					}
                }				
			}

            std::string GrabberModel::toString() {
                std::string result("grabber: " ) ;
                result += "angle " + std::to_string(angle_) ;
                return result ;
            }

            void GrabberModel::run(double dt) {
                double da = voltage_ * dt * degrees_per_second_ ;
                angle_ += da ;

                if (angle_ < cube_angle_)
                    angle_ = cube_angle_ ;

                if (enc_ != nullptr) {
                    int encval = static_cast<int>(angle_ / degrees_per_tick_) + encoder_base_ ;
                    enc_->SimulatorSetValue(encval) ;
                }
            }

            void GrabberModel::inputChanged(SimulatedObject *obj) {
                std::lock_guard<std::mutex> lock(getLockMutex()) ;
                VictorSP *victor = dynamic_cast<VictorSP *>(obj) ;
			    if (victor != nullptr) {
                    voltage_ = victor->Get() ;
                }
            }

	        void GrabberModel::addVictorSP(frc::VictorSP *motor) {
                if (motor->GetChannel() == motor_channel_) {
                    motor_ = motor ;
                    motor_->addModel(this) ;
                }
            }

	        void GrabberModel::addEncoder(frc::Encoder *encoder) {
                int first, second ;

                encoder->SimulatorGetDigitalIOs(first, second) ;
                if ((first == encoder_input_1_ && second == encoder_input_2_) || (first == encoder_input_2_ && second == encoder_input_1_)) {
                    enc_ = encoder ;
                    enc_->addModel(this) ;
                    enc_->SimulatorSetValue(encoder_base_) ;
                }
            }
        }
    }
}
