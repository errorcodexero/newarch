#include "TurnTableModel.h"
#include <frc/RobotSimBase.h>
using namespace frc ;

namespace xero {
    namespace sim {
        namespace phaser {
            TurnTableModel::TurnTableModel(RobotSimBase &simbase) : SubsystemModel(simbase, "lifter") {
                limit_window_ = simbase.getSettingsParser().getDouble("turntable:sim:limit_window") ;

                angle_ = 0.0 ;

                degrees_per_tick_ = simbase.getSettingsParser().getDouble("turntable:sim:degrees_per_tick") ;
                degrees_per_sec_per_volt_ = simbase.getSettingsParser().getDouble("turntable:sim:degrees_per_sec_per_volt") ;

                encoder_1_ = simbase.getSettingsParser().getInteger("hw:turntable:encoder1") ;
                encoder_2_ = simbase.getSettingsParser().getInteger("hw:turntable:encoder2") ;
                motor_1_ = simbase.getSettingsParser().getInteger("hw:turntable:motor") ;

                limit_switch_ = nullptr ;
                power_ = 0.0 ;
                enc_ = nullptr ;
            }

            TurnTableModel::~TurnTableModel() {
            }

            std::string TurnTableModel::toString() {
                std::string result("turntable: ") ;

                result += "angle " + std::to_string(angle_) ;
                return result ;
            }

            void TurnTableModel::run(double dt) {
                double dh = power_ * degrees_per_sec_per_volt_ * dt ;
                angle_ += dh ;

                if (angle_ < 0.0)
                    angle_ = 0.0 ;

                int encval = static_cast<int>(angle_ / degrees_per_tick_) ;
                if (enc_ != nullptr)
                    enc_->SimulatorSetValue(encval) ;

                if (angle_ < limit_window_)
                    limit_switch_->SimulatorSetValue(false) ;                    
                else
                    limit_switch_->SimulatorSetValue(true) ;
            }

            void TurnTableModel::inputChanged(SimulatedObject *obj) {
              
                std::lock_guard<std::mutex> lock(getLockMutex()) ;
                ctre::phoenix::motorcontrol::can::TalonSRX *talon = dynamic_cast<ctre::phoenix::motorcontrol::can::TalonSRX *>(obj) ;
                if (talon != nullptr)
                    power_ = talon->Get() ;
            }
            void TurnTableModel::addTalonSRX(ctre::phoenix::motorcontrol::can::TalonSRX *motor) {
                if (motor->GetDeviceID() == motor_1_) {
                    motor1_ = motor ;
                    motor1_->addModel(this) ;
                }
            }

            void TurnTableModel::addEncoder(frc::Encoder *encoder) {
                int first, second ;

                encoder->SimulatorGetDigitalIOs(first, second) ;
                if ((first == encoder_1_ && second == encoder_2_) || (first == encoder_2_ && second == encoder_1_)) {
                    enc_ = encoder ;
                    enc_->addModel(this) ;
                    enc_->SimulatorSetValue(0) ;
                }
            }

            void TurnTableModel::addDigitalInput(frc::DigitalInput *input)  {
                if (input->GetChannel() == limit_channel_) {
                    limit_switch_ = input ;
                    limit_switch_->addModel(this) ;
                    limit_switch_->SimulatorSetValue(true) ;
                }
            }
        }
    }
}
