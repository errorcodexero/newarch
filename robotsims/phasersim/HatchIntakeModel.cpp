#include "HatchIntakeModel.h"
#include <frc/RobotSimBase.h>

using namespace frc ;
using namespace xero::sim ;
using namespace ctre::phoenix::motorcontrol::can ;

namespace xero {
    namespace sim {
        namespace phaser {
            HatchIntakeModel::HatchIntakeModel(RobotSimBase &simbase) : SubsystemModel(simbase, "hatchintake") {

                motor_channel_ = simbase.getSettingsParser().getInteger("hw:hatchintake:motor") ;
                hatch_sensor_channel_ = simbase.getSettingsParser().getInteger("hw:hatchintake:sensor") ;
                deploy_channel_ = simbase.getSettingsParser().getInteger("hw:hatchintake:deploy") ;

                has_hatch_ = false ;
                deploy_state_ = false ;
                power_ = 0.0 ;
            }

            HatchIntakeModel::~HatchIntakeModel() {
            }

            void HatchIntakeModel::generateDisplayInformation(std::list<std::string> &lines) {

                std::string line ;

                lines.push_back("  Motor: " + std::to_string(getPower())) ;

                line = "  Hatch: " ;
                line += (hasHatch() ? "YES" : "NO") ;
                lines.push_back(line) ;            

                line = "  Deployed: " ;
                line += (isDeployed() ? "YES" : "NO") ;
                lines.push_back(line) ;  
            }

            std::string HatchIntakeModel::toString() {
                std::string result("hatchintake: ") ;
                return result ;
            }

            void HatchIntakeModel::run(double dt) {
            }

            void HatchIntakeModel::inputChanged(SimulatedObject *obj) {
                std::lock_guard<std::mutex> lock(getLockMutex()) ;
                TalonSRX *talon = dynamic_cast<TalonSRX *>(obj) ;
                if (talon != nullptr)
                    power_ = talon->Get() ;

                frc::Solenoid *sol = dynamic_cast<frc::Solenoid *>(obj) ;
                if (sol != nullptr) 
                    deploy_state_ = sol->Get() ;
            }

            void HatchIntakeModel::addTalonSRX(TalonSRX *motor) {
                if (motor->GetDeviceID() == motor_channel_) {
                    motor_ = motor ;
                    motor->addModel(this) ;
                }            
            }
            
            void HatchIntakeModel::addDigitalInput(frc::DigitalInput *input)  {
                if (input->GetChannel() == hatch_sensor_channel_) {
                    hatch_sensor_ = input ;
                    hatch_sensor_->addModel(this) ;
                    hatch_sensor_->SimulatorSetValue(true) ;
                }
            }

            void HatchIntakeModel::addSolenoid(frc::Solenoid *sol) {
                if (sol->SimulatorGetChannel() == deploy_channel_) {
                    deploy_solenoid_ = sol ;
                    deploy_solenoid_->addModel(this) ;
                    deploy_state_ = deploy_solenoid_->Get() ;                    
                }
            }
        }
    }
}
