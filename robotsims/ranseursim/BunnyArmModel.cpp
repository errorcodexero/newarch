#include "BunnyArmModel.h"
#include <frc/RobotSimBase.h>

using namespace frc ;
using namespace xero::sim ;
using namespace ctre::phoenix::motorcontrol::can ;

namespace xero {
    namespace sim {
        namespace ranseur {

            BunnyArmModel::BunnyArmModel(RobotSimBase &simbase) : SubsystemModel(simbase, "bunnyarm") {
                bunnyarm_solenoid_number_ = simbase.getSettingsParser().getInteger("hw:bunnyarm:solenoid") ;
                bunnyarm_state_ = false ;
            }

            BunnyArmModel::~BunnyArmModel() {
            }

            void BunnyArmModel::generateDisplayInformation(std::list<std::string> &lines) {
                std::string line ;
                line = std::string("  BunnyArm: ") + (bunnyarm_state_ ? "UP" : "DOWN") ;
                lines.push_back(line) ;
            }

            std::string BunnyArmModel::toString() {
                std::string result(std::string("tubmanipulator: ") + (bunnyarm_state_ ? "UP" : "DOWN")) ;
                return result ;
            }

            void BunnyArmModel::run(double dt) {
            }

            void BunnyArmModel::inputChanged(SimulatedObject *obj) {
                std::lock_guard<std::mutex> lock(getLockMutex()) ;
                Solenoid *solenoid = dynamic_cast<Solenoid *>(obj) ;
                bunnyarm_state_ = solenoid->Get() ;
            }

            void BunnyArmModel::addDevice(frc::Solenoid *solenoid)  {
                if (solenoid->SimulatorGetChannel() == bunnyarm_solenoid_number_)
                    solenoid->addModel(this) ;
            }
        }
    }
}
