#include "TeleopController.h"
#include <Robot.h>
#include <OISubsystem.h>
#include <MessageLogger.h>

using namespace xero::misc ;

namespace xero {
    namespace base {
        TeleopController::TeleopController(Robot &robot) : ControllerBase(robot) {
            seq_ = std::make_shared<ActionSequence>(robot.getMessageLogger(), "teleop") ;
        }

        TeleopController::~TeleopController() {            
        }

        void TeleopController::run() {
            auto oi = getRobot().getOI() ;

            seq_->clear() ;
            oi->generateActions(seq_) ;
            seq_->start() ;
            seq_->run() ;
            if (!seq_->isDone()) {
                MessageLogger &logger = getRobot().getMessageLogger() ;
                logger.startMessage(MessageLogger::MessageType::error) ;
                logger << "telop sequence did not complete in one cycle" ;
                logger.endMessage() ;
                logger.startMessage(MessageLogger::MessageType::error) ;
                logger << "Sequence: " << seq_->toString() ;
                logger.endMessage() ;
            }           
        }
    }
}