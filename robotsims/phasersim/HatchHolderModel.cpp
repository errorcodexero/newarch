#include "HatchHolderModel.h"
#include <frc/RobotSimBase.h>
#include <frc/SimEvent.h>

using namespace frc ;
using namespace xero::sim ;

namespace xero {
    namespace sim {
        namespace phaser {
            HatchHolderModel::HatchHolderModel(RobotSimBase &simbase) : SubsystemModel(simbase, "hatchholder") {

                arm_retract_channel_ = simbase.getSettingsParser().getInteger("hw:hatchholder:arm:retract") ;
                arm_extend_channel_ = simbase.getSettingsParser().getInteger("hw:hatchholder:arm:extend") ;                
                finger_channel_ = simbase.getSettingsParser().getInteger("hw:hatchholder:finger") ;
                hatch_sensor_channel_ = simbase.getSettingsParser().getInteger("hw:hatchholder:sensor") ;

                has_hatch_ = false ;
                hatch_sensor_ = nullptr ;
            }

            HatchHolderModel::~HatchHolderModel() {
            }

            bool HatchHolderModel::processEvent(const std::string &event, int value) {
                bool ret = false ;
                if (event == "hatch") {
                    ret = true ;
                    has_hatch_ = (value ? true : false) ;
                    if (hatch_sensor_ != nullptr)
                        hatch_sensor_->SimulatorSetValue(!has_hatch_) ;
                }
                return ret ;
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
                    if (sol == arm_extend_solenoid_)
                        extend_state_ = sol->Get() ;
                    else if (sol == arm_retract_solenoid_)
                        retract_state_ = sol->Get() ;
                    else if (sol == finger_solenoid_)
                        finger_state_ = sol->Get() ;

                    if (extend_state_ == true && retract_state_ == false)
                        extension_state_ = true ;
                    else if (extend_state_ == false && retract_state_ == true)
                        extension_state_ = false ;
                }
            }
            
            void HatchHolderModel::addDevice(frc::DigitalInput *input)  {
                if (input->GetChannel() == hatch_sensor_channel_) {
                    hatch_sensor_ = input ;
                    hatch_sensor_->addModel(this) ;
                    hatch_sensor_->SimulatorSetValue(!has_hatch_) ;
                }
            }

            void HatchHolderModel::addDevice(frc::Solenoid *sol) {
                if (sol->SimulatorGetChannel() == arm_extend_channel_) {
                    arm_extend_solenoid_ = sol ;
                    arm_extend_solenoid_->addModel(this) ;
                }
                else if (sol->SimulatorGetChannel() == arm_retract_channel_) {
                    arm_retract_solenoid_ = sol ;
                    arm_retract_solenoid_->addModel(this) ;                    
                }
                else if (sol->SimulatorGetChannel() == finger_channel_) {
                    finger_solenoid_ = sol ;
                    finger_solenoid_->addModel(this) ;
                }
            }
        }
    }
}
