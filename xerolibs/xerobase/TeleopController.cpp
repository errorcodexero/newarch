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

            //
            // Check for a switch to an auto sequence
            //
            if (running_auto_seq_ == nullptr) {
                for(std::shared_ptr<DetectAutoSequence> autoseq: auto_sequences_) {
                    if (autoseq->isTakeoverValid()) {
                        running_auto_seq_ = autoseq->getSequence() ;
                        running_auto_seq_->start() ;
                        oi->getDriverGamepad()->enable(false) ;
                    }
                }
            }

            if (running_auto_seq_ != nullptr) {
                if (oi->getDriverGamepad()->isCancelPressed()) {
                    running_auto_seq_->cancel() ;
                    running_auto_seq_ = nullptr ;
                    oi->getDriverGamepad()->enable(true) ;
                }
                else {
                    //
                    // Run the auto sequence
                    //
                    running_auto_seq_->run() ;
                
                    //
                    // We are running an auto sequence
                    //
                    if (running_auto_seq_->isDone()) {
                        running_auto_seq_ = nullptr ;
                        oi->getDriverGamepad()->enable(true) ;
                    }
                }
            }
            else {                
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
}