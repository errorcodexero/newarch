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
            arm_ = std::make_shared<frc::Solenoid>(robot.getSettingsParser().getInteger("hw:hatchholder:arm"));
            finger_ = std::make_shared<frc::Solenoid>(robot.getSettingsParser().getInteger("hw:hatchholder:finger"));
            sensor_ = std::make_shared<frc::DigitalInput>(robot.getSettingsParser().getInteger("hw:hatchholder:sensor"));
        }   

        HatchHolder::~HatchHolder() {
        }   

        bool HatchHolder::canAcceptAction(xero::base::ActionPtr action) {
            auto coll = std::dynamic_pointer_cast<HatchHolderAction>(action) ;
            return coll != nullptr ;            

        }  

        void HatchHolder::computeState() {
            has_hatch_ = sensor_->Get() ;

            auto &logger = getRobot().getMessageLogger() ;
            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_HATCH_HOLDER) ;
            logger << "HatchHolder:" ;
            logger << " HasHatch " << has_hatch_ ;
            logger.endMessage() ;            
        }

        void HatchHolder::extendArm() {
            arm_->Set(true) ;
        }

        void HatchHolder::retractArm() {
            arm_->Set(false) ;
        }

        void HatchHolder::extendFinger() {
            finger_->Set(true) ;
        }

        void HatchHolder::retractFinger() {
            finger_->Set(false) ;
        }

        bool HatchHolder::holdingHatch() {
            return sensor_ ->Get() ;
        }

    }
}
