#include <frc/TurnTableModel.h>
#include <frc/RobotSimBase.h>
using namespace frc ;

namespace xero {
    namespace sim {
        TurnTableModel::TurnTableModel(RobotSimBase &simbase) : SubsystemModel(simbase, "turntable") {
            limit_window_ = simbase.getSettingsParser().getDouble("turntable:sim:limit_window") ;
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

        void TurnTableModel::generateDisplayInformation(std::list<std::string> &lines) {
            std::string line ;

            lines.push_back("  Angle: " + std::to_string(getAngle())) ;
            lines.push_back("  Motor: " + std::to_string(getPower())) ;

            line = "  MinLimit: " ;
            line += (getMinLimit() ? "ON" : "OFF") ;
            lines.push_back(line) ;

            line = "  MaxLimit: " ;
            line += (getMaxLimit() ? "ON" : "OFF") ;
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
            double dh = power_ * degrees_per_sec_per_volt_ * dt ;
            angle_ += dh ;

            if (angle_ < 0.0)
                angle_ = 0.0 ;

            int encval = static_cast<int>(angle_ / degrees_per_tick_) ;
            if (enc_ != nullptr)
                enc_->SimulatorSetValue(encval) ;

            if (angle_ < limit_window_)
                min_limit_ = false ;
            else
                min_limit_ = true ;

            if (angle_ > max_limit_boundary_ - limit_window_)
                max_limit_ = false ;
            else
                max_limit_ = true ;

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
