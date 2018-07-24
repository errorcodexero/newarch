#include "GrabberModel.h"

using namespace frc ;

namespace xero {
    namespace sim {
        namespace phoenix {
            GrabberModel::GrabberModel() {
                angle_ = 45.0 ;
                degrees_per_second_ = (98.0 - 45.0) / 2.0 ;
                degrees_per_tick_ = 0.625 ;
                encoder_base_ = 123 ;
                cube_angle_ = -3.0 ;
            }

            GrabberModel::~GrabberModel() {
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
                if (motor->GetChannel() == 2) {
                    motor_ = motor ;
                    motor_->addModel(this) ;
                }
            }

	        void GrabberModel::addEncoder(frc::Encoder *encoder) {
                int first, second ;

                encoder->SimulatorGetDigitalIOs(first, second) ;
                if (first == 6 && second == 7) {
                    enc_ = encoder ;
                    enc_->addModel(this) ;
                    enc_->SimulatorSetValue(encoder_base_) ;
                }
            }
        }
    }
}
