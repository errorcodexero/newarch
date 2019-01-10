#include "LineFollower.h"
#include "LineFollowerAction.h"
#include <Robot.h>
#include <MessageLogger.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phaser {
        LineFollower::LineFollower(Robot &robot, uint64_t mid) : Subsystem(robot, "linefollower") {
            int inid ;

            msg_id_ = mid ;

            std::cout << "Building line follower" << std::endl ;

            inid = robot.getSettingsParser().getInteger("hw:follower:left") ;
            left_ = std::make_shared<frc::DigitalInput>(inid) ;

            inid = robot.getSettingsParser().getInteger("hw:follower:right") ;
            right_ = std::make_shared<frc::DigitalInput>(inid) ;

            inid = robot.getSettingsParser().getInteger("hw:follower:middle") ;
            middle_ = std::make_shared<frc::DigitalInput>(inid) ;
        }

        LineFollower::~LineFollower() {
        }

        bool LineFollower::canAcceptAction(xero::base::ActionPtr action) {
            std::shared_ptr<LineFollowerAction> laction = std::dynamic_pointer_cast<LineFollowerAction>(action) ;
            return laction != nullptr ;
        }

        void LineFollower::computeState() {
            left_state_ = left_->Get() ;
            middle_state_ = middle_->Get() ;
            right_state_ = right_->Get() ;

            MessageLogger &logger = getRobot().getMessageLogger() ;
            logger.startMessage(MessageLogger::MessageType::debug, msg_id_) ;
            logger << "Line Follower State:";
            logger << " " << (left_state_ ? "on" : "off") ;
            logger << " " << (middle_state_ ? "on" : "off") ;
            logger << " " << (right_state_ ? "on" : "off") ;            
            logger.endMessage() ;
        }
    }
}