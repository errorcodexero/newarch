#include "LifterModel.h"

using namespace frc ;

namespace xero {
    namespace sim {
        namespace phoenix {
            LifterModel::LifterModel() {
                height_ = 11.375 ;
                bottom_height_ = 11.375 ;
                brake_value_ = true ;
                highgear_ = true ;
                in_per_tick_ = 0.08327 ;

                inch_per_sec_per_volt = 78.0 / 2.0 ;
                encoder_base_ = 1111 ;
            }

            LifterModel::~LifterModel() {
            }

            std::string LifterModel::toString() {
                std::string result("lifter: ") ;

                result += "height " + std::to_string(height_) ;
                result += ", brake "  ;
                result += (brake_value_ ? "ON" : "OFF") ;
                result += ", gear "  ;
                result += (gear_ ? "HIGH" : "LOW") ;
                result += ", top " ;
                if (top_limit_ != nullptr)
                    result += (top_limit_->Get() ? "ON" : "OFF") ;
                else
                    result += "UNASSIGNED" ;

                result += ", bottom " ;
                if (bottom_limit_ != nullptr)
                    result += (bottom_limit_->Get() ? "ON" : "OFF") ;
                else
                    result += "UNASSIGNED" ;

                return result ;
            }

            void LifterModel::run(double dt) {
                if (!brake_value_) {
                    double dh = voltage_ * inch_per_sec_per_volt * dt ;
                    height_ += dh ;

                    if (height_ <= 11.375) {
                        height_ = 11.375 ;
                        if (bottom_limit_ != nullptr)
                            bottom_limit_->SimulatorSetValue(false) ;
                    }
                    else {
                        if (bottom_limit_ != nullptr)
                            bottom_limit_->SimulatorSetValue(true) ;
                    }

                    if (height_ >= 88.6) {
                        height_ = 88.6 ;
                        if (top_limit_ != nullptr)
                            top_limit_->SimulatorSetValue(false) ;
                    }
                    else {
                        if (top_limit_ != nullptr)
                            top_limit_->SimulatorSetValue(true) ;
                    }

                    int encval = static_cast<int>((height_ - bottom_height_) / in_per_tick_) + encoder_base_ ;
                    enc_->SimulatorSetValue(encval) ;
                }
            }

	        void LifterModel::inputChanged(SimulatedObject *obj) {
              
			    std::lock_guard<std::mutex> lock(getLockMutex()) ;
                VictorSP *victor = dynamic_cast<VictorSP *>(obj) ;
			    if (victor != nullptr) {
                    voltage_ = victor->Get() ;
                }

                Solenoid *sol = dynamic_cast<Solenoid *>(obj) ;
                if (sol != nullptr) {
                    if (sol == brake_) {
                        brake_value_ = !sol->Get() ;
                    }
                    else if (sol == gear_) {
                        highgear_ = !sol->Get() ;
                    }
                }
            }

	        void LifterModel::addVictorSP(frc::VictorSP *motor) {
                if (motor->GetChannel() == 0) {
                    motor1_ = motor ;
                    motor1_->addModel(this) ;
                }
                else if (motor->GetChannel() == 1) {
                    motor2_ = motor ;
                    motor2_->addModel(this) ;
                }
            }

	        void LifterModel::addEncoder(frc::Encoder *encoder) {
                int first, second ;

                encoder->SimulatorGetDigitalIOs(first, second) ;
                if (first == 4 && second == 5) {
                    enc_ = encoder ;
                    enc_->addModel(this) ;
                    enc_->SimulatorSetValue(encoder_base_) ;
                }
            }

            void LifterModel::addDigitalInput(frc::DigitalInput *input)  {
                if (input->GetChannel() == 8) {
                    top_limit_ = input ;
                    top_limit_->addModel(this) ;
                    top_limit_->SimulatorSetValue(true) ;
                }

                if (input->GetChannel() == 9) {
                    bottom_limit_ = input ;
                    bottom_limit_->addModel(this) ;
                    bottom_limit_->SimulatorSetValue(true) ;
                }
            }

            void LifterModel::addSolenoid(frc::Solenoid *solenoid) {
                if (solenoid->SimulatorGetChannel() == 1) {
                    gear_ = solenoid ;
                    gear_->addModel(this) ;
                }

                if (solenoid->SimulatorGetChannel() == 3) {
                    brake_ = solenoid ;
                    brake_->addModel(this) ;
                }
            }
        }
    }
}