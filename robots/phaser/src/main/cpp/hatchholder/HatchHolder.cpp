#include "HatchHolder.h"
#include <Robot.h>
#include <ActionSequence.h>
#include <MessageLogger.h>
#include "HatchHolderAction.h"
#include "phaserids.h"


using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phaser {
        HatchHolder::HatchHolder(xero::base::Robot &robot) : Subsystem(robot, "HatchHolder") {
            arm_extend_ = std::make_shared<frc::Solenoid>(robot.getSettingsParser().getInteger("hw:hatchholder:arm:extend"));
            arm_retract_ = std::make_shared<frc::Solenoid>(robot.getSettingsParser().getInteger("hw:hatchholder:arm:retract"));            
            finger_ = std::make_shared<frc::Solenoid>(robot.getSettingsParser().getInteger("hw:hatchholder:finger"));
            sensor_ = std::make_shared<frc::DigitalInput>(robot.getSettingsParser().getInteger("hw:hatchholder:sensor"));

            duration_ = robot.getSettingsParser().getDouble("hatchholder:debounce") ;

            finger_->Set(false) ;
            arm_extend_->Set(false) ;
            arm_retract_->Set(false) ;

            finger_deployed_ = false ;
            arm_deployed_ = false ;

            auto_hatch_enabled_ = true ;
            
            has_hatch_ = false ;
            last_switch_state_ = false ;
            last_switch_time_ = robot.getTime() ;
            pending_ = false ;
        }   

        HatchHolder::~HatchHolder() {
        }   

        bool HatchHolder::canAcceptAction(xero::base::ActionPtr action) {
            auto coll = std::dynamic_pointer_cast<HatchHolderAction>(action) ;
            return coll != nullptr ;            

        }  

        void HatchHolder::computeState() {
            bool current = sensor_->Get() ;

            if (pending_) {
                if (current != last_switch_state_) {
                    last_switch_state_ = current ;
                    last_switch_time_ = getRobot().getTime() ;
                }
                else if (getRobot().getTime() - last_switch_time_ > duration_) {
                    has_hatch_ = current ;
                    pending_ = false ;
                }
            }
            else
            {
                if (current != has_hatch_) {
                    last_switch_state_ = current ;
                    last_switch_time_ = getRobot().getTime() ;
                    pending_ = true ;
                }
            }

            auto &logger = getRobot().getMessageLogger() ;
            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_HATCH_HOLDER) ;
            logger << "HatchHolder:" ;
            logger << " DIO " << sensor_->GetChannel() ;
            logger << " HasHatch " << has_hatch_ ;
            logger.endMessage() ;       
        }

        void HatchHolder::run() {
            Subsystem::run() ;

            if (auto_hatch_enabled_) {
                if (hasHatch() && !isFingerDeployed())
                    extendFinger() ;
                else if (!hasHatch() && isFingerDeployed())
                    retractFinger() ;
            }
        }

        void HatchHolder::extendArm() {
            arm_extend_->Set(true) ;
            arm_retract_->Set(false) ;
            arm_deployed_ = true ;
        }

        void HatchHolder::retractArm() {
            arm_extend_->Set(false) ;
            arm_retract_->Set(true) ;
            arm_deployed_ = false ;
        }

        void HatchHolder::stopArm() {
            arm_extend_->Set(false) ;
            arm_retract_->Set(false) ;            
        }

        void HatchHolder::extendFinger() {
            finger_->Set(true) ;
            finger_deployed_ = true ;
        }

        void HatchHolder::retractFinger() {
            finger_->Set(false) ;
            finger_deployed_ = false ;
        }

        bool HatchHolder::holdingHatch() {
            return sensor_ ->Get() ;
        }

    }
}
