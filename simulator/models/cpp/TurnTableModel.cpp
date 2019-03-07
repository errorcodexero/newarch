#include <TurnTableModel.h>
#include <frc/RobotSimBase.h>
#include <xeromath.h>
#include <MessageLogger.h>
#include <cmath>

using namespace frc ;
using namespace xero::misc ;

namespace xero {
    namespace sim {
        TurnTableModel::TurnTableModel(RobotSimBase &simbase) : SubsystemModel(simbase, "turntable") {
            min_limit_boundary_ = simbase.getSettingsParser().getDouble("turntable:sim:min_limit_boundary") ;
            max_limit_boundary_ = simbase.getSettingsParser().getDouble("turntable:sim:max_limit_boundary") ;

            angle_ = 0.0 ;

            degrees_per_tick_ = simbase.getSettingsParser().getDouble("turntable:sim:degrees_per_tick") ;
            degrees_per_sec_per_volt_ = simbase.getSettingsParser().getDouble("turntable:sim:degrees_per_sec_per_volt") ;

            encoder_1_ = simbase.getSettingsParser().getInteger("hw:turntable:encoder1") ;
            encoder_2_ = simbase.getSettingsParser().getInteger("hw:turntable:encoder2") ;
            motor_1_ = simbase.getSettingsParser().getInteger("hw:turntable:motor") ;

            min_limit_switch_ = nullptr ;
            max_limit_switch_ = nullptr ;
            power_ = 0.0 ;
            enc_ = nullptr ;
        }

        TurnTableModel::~TurnTableModel() {
        }

        bool TurnTableModel::processEvent(const std::string &name, int value) {
            bool ret = false ;
            if (name == "verify") {
                ret = true ;
                double angle = static_cast<double>(value) / 100.0 ;
                double err = std::fabs(angle - getAngle()) ;
                if (err > 2.0) {
                    MessageLogger &logger = getRobotMessageLogger() ;
                    logger.startMessage(MessageLogger::MessageType::error) ;
                    logger << "VerifyError: turntable" ;
                    logger << " expected " << angle ;
                    logger << " actual " << getAngle() ;
                    logger.endMessage() ;
                }
            }

            return ret ;
        }

        void TurnTableModel::generateDisplayInformation(std::list<std::string> &lines) {
            std::string line ;

            lines.push_back("  Angle: " + std::to_string(getAngle())) ;
            lines.push_back("  Motor: " + std::to_string(getPower())) ;

            line = "  MinLimit: " ;
            line += (getMinLimit() ? "ON" : "OFF") ;
            lines.push_back(line) ;

            line = "  MaxLimit: " ;
            line += (getMaxLimit() ? "ON" : "OFF") ;
            lines.push_back(line) ;
            
            line = "  Encoder: " ;
            line += std::to_string(encoder_value_) ;
            lines.push_back(line) ;
        }

        std::string TurnTableModel::toString() {
            std::string result("turntable: ") ;

            result += "angle " + std::to_string(angle_) ;
            result += ", power " + std::to_string(power_) ;
            result += ", min_limit "  ;
            result += (min_limit_ ? "OFF" : "ON") ;
            result += ", max_limit "  ;
            result += (max_limit_ ? "OFF" : "ON") ;
            return result ;
        }

        void TurnTableModel::run(double dt) {
            //
            // Note, this only works for the phaser turntable
            //
            double speed = power_ * degrees_per_sec_per_volt_;
            double ddeg = speed * dt ;
            angle_ = xero::math::normalizeAngleDegrees(angle_ + ddeg) ;

            double encangle = angle_ ;
            if (angle_ < max_limit_boundary_)
                encangle += 360.0 ;

            encoder_value_ = static_cast<int>(encangle / degrees_per_tick_) ;
            if (enc_ != nullptr)
                enc_->SimulatorSetValue(-encoder_value_) ;

            min_limit_ = false ;
            max_limit_ = false ;

            if (angle_ <= max_limit_boundary_ && angle_ >= min_limit_boundary_) {
                //
                // We are at a boundary, set a limit switch and restrict movement
                // to the mechanical hard stop
                //
                double d1 = std::fabs(angle_ - max_limit_boundary_) ;
                double d2 = std::fabs(angle_ - min_limit_boundary_) ;

                if (d1 < d2) {
                    // Closest to the max_boundary
                    angle_ = max_limit_boundary_ ;
                    min_limit_ = true ;
                }
                else {
                    // Closest to the min_boundary
                    angle_ = min_limit_boundary_ ;
                    max_limit_ = true ;
                }
            }

            if (min_limit_switch_ != nullptr)
                min_limit_switch_->SimulatorSetValue(min_limit_) ;

            if (max_limit_switch_ != nullptr)
                max_limit_switch_->SimulatorSetValue(max_limit_) ;
        }

        void TurnTableModel::inputChanged(SimulatedObject *obj) {
            std::lock_guard<std::mutex> lock(getLockMutex()) ;
            ctre::phoenix::motorcontrol::can::TalonSRX *talon = dynamic_cast<ctre::phoenix::motorcontrol::can::TalonSRX *>(obj) ;
            if (talon != nullptr)
                power_ = talon->Get() ;
        }

        void TurnTableModel::addDevice(ctre::phoenix::motorcontrol::can::TalonSRX *motor) {
            if (motor->GetDeviceID() == motor_1_) {
                motor1_ = motor ;
                motor1_->addModel(this) ;
            }
        }

        void TurnTableModel::addDevice(frc::Encoder *encoder) {
            int first, second ;

            encoder->SimulatorGetDigitalIOs(first, second) ;
            if ((first == encoder_1_ && second == encoder_2_) || (first == encoder_2_ && second == encoder_1_)) {
                enc_ = encoder ;
                enc_->addModel(this) ;
                enc_->SimulatorSetValue(0) ;
            }
        }

        void TurnTableModel::addDevice(frc::DigitalInput *input)  {
            if (input->GetChannel() == min_limit_channel_) {
                min_limit_switch_ = input ;
                min_limit_switch_->addModel(this) ;
                min_limit_switch_->SimulatorSetValue(true) ;
            }
            else if (input->GetChannel() == max_limit_channel_) {
                max_limit_switch_ = input ;
                max_limit_switch_->addModel(this) ;
                max_limit_switch_->SimulatorSetValue(true) ;
            }
        }
    }
}
