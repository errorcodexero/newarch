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

                deploy_state_ = false ;
                power_ = 0.0 ;
            }

            CargoIntakeModel::~CargoIntakeModel() {
            }

            void CargoIntakeModel::generateDisplayInformation(std::list<std::string> &lines) {

                std::string line ;

                lines.push_back("  Motor: " + std::to_string(getPower())) ;

                line = "  Deployed: " ;
                line += (isDeployed() ? "YES" : "NO") ;
                lines.push_back(line) ;  
            }

            std::string CargoIntakeModel::toString() {
                std::string result("hatchintake: ") ;
                return result ;
            }

            void CargoIntakeModel::run(double dt) {
            }

            void CargoIntakeModel::inputChanged(SimulatedObject *obj) {
                std::lock_guard<std::mutex> lock(getLockMutex()) ;
                TalonSRX *talon = dynamic_cast<TalonSRX *>(obj) ;
                if (talon != nullptr)
                    power_ = talon->Get() ;

                frc::Solenoid *sol = dynamic_cast<frc::Solenoid *>(obj) ;
                if (sol != nullptr) 
                    deploy_state_ = sol->Get() ;                    
            }

            void CargoIntakeModel::addTalonSRX(TalonSRX *motor) {
                if (motor->GetDeviceID() == motor_channel_) {
                    motor_ = motor ;
                    motor->addModel(this) ;
                }            
            }
            
            void CargoIntakeModel::addSolenoid(frc::Solenoid *sol) {
                if (sol->SimulatorGetChannel() == deploy_channel_) {
                    deploy_solenoid_ = sol ;
                    deploy_solenoid_->addModel(this) ;
                    deploy_state_ = deploy_solenoid_->Get() ;                    
                }
            }
        }
    }
}
