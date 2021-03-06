#include "TubManipulatorModel.h"
#include <frc/RobotSimBase.h>

using namespace frc ;
using namespace xero::sim ;
using namespace xero::misc ;
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
                arm_degrees_per_volt_per_sec_ = ArmDegreesPerVoltPerSecond ;

                arm_motor_power_ = 0.0 ;
                wrist_motor_power_ = 0.0 ;
                collector_motor_power_ = 0.0 ;
                arm_angle_ = ArmMaximumAngle ;
                wrist_angle_ = WristMaxAngle ;
                collector_sensor_value_ = false ;
                collector_state_ = false ;
                has_tub_inv_ = false ;

                arm_encoder_ = nullptr ;
                wrist_encoder_ = nullptr ;
                tub_sensor_ = nullptr ;

                double ec, rc ;

                arm_mapper_ = new EncoderMapper(180.0, -180.0, 5.0, 0.0) ;
                ec = simbase.getSettingsParser().getDouble("hw:tubmanipulator:arm:ec") ;
                rc = simbase.getSettingsParser().getDouble("hw:tubmanipulator:arm:rc") ;                
                arm_mapper_->calibrate(rc, ec) ;

                wrist_mapper_ = new EncoderMapper(180.0, -180.0, 0.0, 5.0) ;
                ec = simbase.getSettingsParser().getDouble("hw:tubmanipulator:wrist:ec") ;
                rc = simbase.getSettingsParser().getDouble("hw:tubmanipulator:wrist:rc") ;                 
                wrist_mapper_->calibrate(rc, ec) ;
            }

            TubManipulatorModel::~TubManipulatorModel() {
                delete arm_mapper_ ;
                delete wrist_mapper_ ;
            }

            bool TubManipulatorModel::processEvent(const std::string &name, int value) {
                bool ret = false ;
                if (name == "tub") {
                    ret = true ;
                    has_tub_inv_ = (value ? true : false) ;
                    setTubSensor() ;
                }
                else if (name == "arm")
                {
                    ret = true ;
                    arm_angle_ = static_cast<double>(value) ;
                    setArmEncoder() ;
                }
                else if (name == "wrist")
                {
                    ret = true ;
                    wrist_angle_ = static_cast<double>(value) ;
                    setWristEncoder() ;
                }

                return ret ;
            }

            void TubManipulatorModel::generateDisplayInformation(std::list<std::string> &lines) {
                std::string line ;

                lines.push_back("------------------------------") ;
                lines.push_back("  ArmMotor: " + std::to_string(arm_motor_power_)) ;
                lines.push_back("  ArmAngle: " + std::to_string(arm_angle_)) ;
                if (arm_encoder_ != nullptr)
                    lines.push_back("  ArmEncoder: " + std::to_string(arm_encoder_->GetVoltage())) ;
                else
                    lines.push_back("  ArmEncoder: ") ;
                lines.push_back("------------------------------") ;
                lines.push_back("  WristMotor: " + std::to_string(wrist_motor_power_)) ;
                lines.push_back("  WristAngle: " + std::to_string(wrist_angle_)) ;
                if (wrist_encoder_ != nullptr)
                    lines.push_back("  WristEncoder: " + std::to_string(wrist_encoder_->GetVoltage())) ;
                else
                    lines.push_back("  WristEncoder: ") ;          
                lines.push_back("------------------------------") ;                          
                lines.push_back("  CollMotor: " + std::to_string(collector_motor_power_)) ;
                line = "  CollArms: " ;
                line += (collector_state_ ? "OPEN" : "CLOSED") ;
                lines.push_back(line) ;                  
                line = "  Tub: " ;
                line += (hasTub() ? "YES" : "NO") ;
                lines.push_back(line) ;            
            }

            std::string TubManipulatorModel::toString() {
                std::string result("tubmanipulator: ") ;
                return result ;
            }

            void TubManipulatorModel::run(double dt) {
                arm_angle_ += arm_degrees_per_volt_per_sec_ * arm_motor_power_ * dt ;
                if (arm_angle_ > ArmMaximumAngle)
                    arm_angle_ = ArmMaximumAngle ;
                else if (arm_angle_ < ArmMinimumAngle)
                    arm_angle_ = ArmMinimumAngle ;

                wrist_angle_ += wrist_degrees_per_volt_per_sec_ * wrist_motor_power_ * dt ;
#ifdef NOTYET
                if (wrist_angle_ > WristMaxAngle)
                    wrist_angle_ = WristMaxAngle ;
                else if (wrist_angle_ < WristMinAngle)
                    wrist_angle_ = WristMinAngle ;
#endif

                setArmEncoder() ;
                setWristEncoder() ;

                if (collector_motor_power_ > 0.2 && !has_tub_inv_)
                {
                    has_tub_inv_ = true ;
                    setTubSensor() ;
                }
            }

            void TubManipulatorModel::setTubSensor()
            {
                if (tub_sensor_ != nullptr)
                    tub_sensor_->SimulatorSetValue(has_tub_inv_) ;
            }

            void TubManipulatorModel::setArmEncoder() {
                if (arm_encoder_ != nullptr)
                {
                    double v = arm_mapper_->toEncoder(arm_angle_) ;
                    arm_encoder_->SimulatorSetVoltage(v) ;
                }
            }

            void TubManipulatorModel::setWristEncoder() {
                if (wrist_encoder_ != nullptr)
                {
                    double v = wrist_mapper_->toEncoder(wrist_angle_) ;
                    wrist_encoder_->SimulatorSetVoltage(v) ;                
                }
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

                Solenoid *sol = dynamic_cast<Solenoid *>(obj) ;
                if (sol != nullptr)
                {
                    collector_state_ = sol->Get() ;
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
                    tub_sensor_->SimulatorSetValue(has_tub_inv_) ;
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
