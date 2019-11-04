#include "TubManipulatorModel.h"
#include <frc/RobotSimBase.h>

using namespace frc ;
using namespace xero::sim ;
using namespace ctre::phoenix::motorcontrol::can ;

namespace xero {
    namespace sim {
        namespace ranseur {

            TubManipulatorModel::TubManipulatorModel(RobotSimBase &simbase) : SubsystemModel(simbase, "tubmanipulator") {
                arm_motor_channel_ = simbase.getSettingsParser().getInteger("hw:tubmanipulator:arm:motor") ;
                arm_encoder_channel_ = simbase.getSettingsParser().getInteger("hw:tubmanipulator:arm:encoder") ;

                wrist_motor_channel_ = simbase.getSettingsParser().getInteger("hw:tubmanipulator:wrist:motor") ;
                wrist_encoder_channel_ = simbase.getSettingsParser().getInteger("hw:tubmanipulator:wrist:encoder") ;

                collector_solenoid_number_ = simbase.getSettingsParser().getInteger("hw:tubmanipulator:collector:solenoid") ;
                collector_tub_sensor_channel_ = simbase.getSettingsParser().getInteger("hw:tubmanipulator:collector:sensor") ;
                collector_motor_channel_ = simbase.getSettingsParser().getInteger("hw:tubmanipulator:collector:motor") ;

                wrist_degrees_per_volt_per_sec_ = WristDegreesPerVoltPerSecond ;
                arm_degress_per_volt_per_sec_ = ArmDegreesPerVoltPerSecond ;

                arm_motor_power_ = 0.0 ;
                wrist_motor_power_ = 0.0 ;
                collector_motor_power_ = 0.0 ;
                arm_angle_ = ArmMaximumAngle ;
                wrist_angle_ = WristMinAngle ;
                collector_sensor_value_ = false ;
                collector_state_ = false ;
                has_tub_ = false ;
            }

            TubManipulatorModel::~TubManipulatorModel() {
            }

            bool TubManipulatorModel::processEvent(const std::string &name, int value) {
                bool ret = false ;
                if (name == "tub") {
                    ret = true ;
                    has_tub_ = (value ? true : false) ;
                    if (tub_sensor_ != nullptr)
                        tub_sensor_->SimulatorSetValue(has_tub_) ;
                }

                return ret ;
            }

            void TubManipulatorModel::generateDisplayInformation(std::list<std::string> &lines) {
                std::string line ;

                lines.push_back("  ArmMotor: " + std::to_string(arm_motor_power_)) ;
                lines.push_back("  ArmAngle: " + std::to_string(arm_angle_)) ;
                lines.push_back("  WristMotor: " + std::to_string(wrist_motor_power_)) ;
                lines.push_back("  WristAngle: " + std::to_string(wrist_angle_)) ;
                lines.push_back("  CollMotor: " + std::to_string(collector_motor_power_)) ;
                line = "  CollArms: " ;
                line += (collector_state_ ? "OPEN" : "CLOSED") ;
                line = "  Tub: " ;
                line += (hasTub() ? "YES" : "NO") ;
                lines.push_back(line) ;            
            }

            std::string TubManipulatorModel::toString() {
                std::string result("tubmanipulator: ") ;
                return result ;
            }

            void TubManipulatorModel::run(double dt) {
            }

            void TubManipulatorModel::setArmEncoder() {
                std::lock_guard<std::mutex> lock(getLockMutex()) ;
                double v = arm_angle_ / 360.0 * 1024 ;
                arm_encoder_->SimulatorSetVoltage(v) ;
            }

            void TubManipulatorModel::setWristEncoder() {
                std::lock_guard<std::mutex> lock(getLockMutex()) ;
                double v = wrist_angle_ / 360.0 * 1024 ;
                wrist_encoder_->SimulatorSetVoltage(v) ;                
            }

            void TubManipulatorModel::inputChanged(SimulatedObject *obj) {
                std::lock_guard<std::mutex> lock(getLockMutex()) ;
                VictorSPX *victor = dynamic_cast<VictorSPX *>(obj) ;
                if (victor != nullptr)
                {
                    if (victor->GetDeviceID() == arm_motor_channel_)
                        arm_motor_power_ = victor->Get() ;
                    else if (victor->GetDeviceID() == wrist_motor_channel_)
                        wrist_motor_power_ = victor->Get() ;
                    else if (victor->GetDeviceID() == collector_motor_channel_)
                        collector_motor_power_ = victor->Get() ;
                }
            }

            void TubManipulatorModel::addDevice(VictorSPX *motor) {
                int id = motor->GetDeviceID() ;
                if (id == arm_motor_channel_ || id == wrist_motor_channel_ || id == collector_motor_channel_)
                    motor->addModel(this) ;
            }
            
            void TubManipulatorModel::addDevice(frc::DigitalInput *input)  {
                if (input->GetChannel() == collector_tub_sensor_channel_) {
                    tub_sensor_ = input ;
                    tub_sensor_->addModel(this) ;
                    tub_sensor_->SimulatorSetValue(has_tub_) ;
                }
            }

            void TubManipulatorModel::addDevice(frc::AnalogInput *input)  {
                if (input->SimulatorGetChannel() == arm_encoder_channel_)
                {
                    arm_encoder_ = input ;
                    arm_encoder_->addModel(this) ;
                    setArmEncoder() ;
                }
                else if (input->SimulatorGetChannel() == wrist_encoder_channel_)
                {
                    wrist_encoder_ = input ;
                    wrist_encoder_->addModel(this) ;
                    setWristEncoder() ;
                }
            }

            void TubManipulatorModel::addDevice(frc::Solenoid *solenoid)  {
                if (solenoid->SimulatorGetChannel() == collector_solenoid_number_)
                    solenoid->addModel(this) ;
            }
        }
    }
}
