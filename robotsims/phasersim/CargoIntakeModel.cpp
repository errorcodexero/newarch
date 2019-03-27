#include "CargoIntakeModel.h"
#include <frc/RobotSimBase.h>

using namespace frc ;
using namespace xero::sim ;
using namespace ctre::phoenix::motorcontrol::can ;

namespace xero {
    namespace sim {
        namespace phaser {
            CargoIntakeModel::CargoIntakeModel(RobotSimBase &simbase) : SubsystemModel(simbase, "cargointake") {

                motor_channel_ = simbase.getSettingsParser().getInteger("hw:cargointake:motor") ;
                deploy_channel_ = simbase.getSettingsParser().getInteger("hw:cargointake:deploy") ;
                cargo_sensor_channel_ = simbase.getSettingsParser().getInteger("hw:cargointake:sensor") ;                

                deploy_state_ = false ;
                power_ = 0.0 ;
                has_cargo_ = true ;
            }

            CargoIntakeModel::~CargoIntakeModel() {
            }

            bool CargoIntakeModel::processEvent(const std::string &name, int value) {
                bool ret = false ;
                if (name == "cargo") {
                    ret = true ;
                    has_cargo_ = (value ? true : false) ;
                    if (cargo_sensor_ != nullptr)
                        cargo_sensor_->SimulatorSetValue(has_cargo_) ;
                }

                return ret ;
            }            

            void CargoIntakeModel::generateDisplayInformation(std::list<std::string> &lines) {

                std::string line ;

                lines.push_back("  Motor: " + std::to_string(getPower())) ;

                line = "  Deployed: " ;
                line += (isDeployed() ? "YES" : "NO") ;
                lines.push_back(line) ;  
            }

            std::string CargoIntakeModel::toString() {
                std::string result("cargointake: ") ;
                return result ;
            }

            void CargoIntakeModel::run(double dt) {
            }

            void CargoIntakeModel::inputChanged(SimulatedObject *obj) {
                std::lock_guard<std::mutex> lock(getLockMutex()) ;
                VictorSPX *victor = dynamic_cast<VictorSPX *>(obj) ;
                if (victor != nullptr)
                    power_ = victor->Get() ;

                frc::Solenoid *sol = dynamic_cast<frc::Solenoid *>(obj) ;
                if (sol != nullptr) 
                    deploy_state_ = sol->Get() ;                    
            }

            void CargoIntakeModel::addDevice(VictorSPX *motor) {
                if (motor->GetDeviceID() == motor_channel_) {
                    motor_ = motor ;
                    motor->addModel(this) ;
                }            
            }
            
            void CargoIntakeModel::addDevice(frc::Solenoid *sol) {
                if (sol->SimulatorGetChannel() == deploy_channel_) {
                    deploy_solenoid_ = sol ;
                    deploy_solenoid_->addModel(this) ;
                    deploy_state_ = deploy_solenoid_->Get() ;                    
                }
            }

            void CargoIntakeModel::addDevice(frc::DigitalInput *input)  {
                if (input->GetChannel() == cargo_sensor_channel_) {
                    cargo_sensor_ = input ;
                    cargo_sensor_->addModel(this) ;
                    cargo_sensor_->SimulatorSetValue(has_cargo_) ;
                }
            }
        }
    }
}
