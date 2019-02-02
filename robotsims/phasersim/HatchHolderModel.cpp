#include "HatchHolderModel.h"
#include <frc/RobotSimBase.h>

using namespace frc ;
using namespace xero::sim ;

namespace xero {
    namespace sim {
        namespace phaser {
            HatchHolderModel::HatchHolderModel(RobotSimBase &simbase) : SubsystemModel(simbase, "hatchholder") {

                extension_channel_ = simbase.getSettingsParser().getInteger("hw:hatchholder:arm") ;
                finger_channel_ = simbase.getSettingsParser().getInteger("hw:hatchholder:finger") ;
                hatch_sensor_channel_ = simbase.getSettingsParser().getInteger("hw:hatchholder:sensor") ;

                has_hatch_ = false ;
            }

            HatchHolderModel::~HatchHolderModel() {
            }

            void HatchHolderModel::generateDisplayInformation(std::list<std::string> &lines) {

                std::string line ;

                line = "  Extension: " ;
                line += (getExtensionState() ? "ON" : "OFF") ;
                lines.push_back(line) ;

                line = "  Finger: " ;
                line += (getFingerState() ? "ON" : "OFF") ;
                lines.push_back(line) ;

                line = "  Hatch: " ;
                line += (hasHatch() ? "YES" : "NO") ;
                lines.push_back(line) ;            
            }

            std::string HatchHolderModel::toString() {
                std::string result("hatchholder: ") ;
                return result ;
            }

            void HatchHolderModel::run(double dt) {
            }

            void HatchHolderModel::inputChanged(SimulatedObject *obj) {
                std::lock_guard<std::mutex> lock(getLockMutex()) ;
                frc::Solenoid *sol = dynamic_cast<frc::Solenoid *>(obj) ;
                if (sol != nullptr) {
                    if (sol == extension_solenoid_)
                        extension_state_ = sol->Get() ;
                    else if (sol == finger_solenoid_)
                        finger_state_ = sol->Get() ;
                }
            }
            
            void HatchHolderModel::addDevice(frc::DigitalInput *input)  {
                if (input->GetChannel() == hatch_sensor_channel_) {
                    hatch_sensor_ = input ;
                    hatch_sensor_->addModel(this) ;
                    hatch_sensor_->SimulatorSetValue(true) ;
                }
            }

            void HatchHolderModel::addDevice(frc::Solenoid *sol) {
                if (sol->SimulatorGetChannel() == extension_channel_) {
                    extension_solenoid_ = sol ;
                    extension_solenoid_->addModel(this) ;
                }

                if (sol->SimulatorGetChannel() == finger_channel_) {
                    finger_solenoid_ = sol ;
                    finger_solenoid_->addModel(this) ;
                }
            }
        }
    }
}
