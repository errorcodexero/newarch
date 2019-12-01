#include "ClimberModel.h"
#include <frc/RobotSimBase.h>

using namespace frc ;
using namespace xero::sim ;
using namespace ctre::phoenix::motorcontrol::can ;

namespace xero {
    namespace sim {
        namespace phaser {
            ClimberModel::ClimberModel(RobotSimBase &simbase) : SubsystemModel(simbase, "climber") {
                deploy_channel_ = simbase.getSettingsParser().getInteger("hw:climber:deploy") ;

                deploy_state_ = false ;
            }

            ClimberModel::~ClimberModel() {
            }

            void ClimberModel::generateDisplayInformation(std::list<std::string> &lines) {

                std::string line ;

                line = "  Deployed: " ;
                line += (isDeployed() ? "YES" : "NO") ;
                lines.push_back(line) ;            
            }

            std::string ClimberModel::toString() {
                std::string result("climber: ") ;
                return result ;
            }

            void ClimberModel::run(double dt) {
            }

            void ClimberModel::inputChanged(SimulatedObject *obj) {
                std::lock_guard<std::mutex> lock(getLockMutex()) ;
                frc::Solenoid *sol = dynamic_cast<frc::Solenoid *>(obj) ;
                if (sol != nullptr) 
                    deploy_state_ = sol->Get() ;
            }

            void ClimberModel::addDevice(frc::Solenoid *sol) {
                if (sol->SimulatorGetChannel() == deploy_channel_) {
                    deploy_solenoid_ = sol ;
                    deploy_solenoid_->addModel(this) ;
                    deploy_state_ = deploy_solenoid_->Get() ;                    
                }
            }
        }
    }
}
