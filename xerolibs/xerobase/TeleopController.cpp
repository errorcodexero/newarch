#include "TeleopController.h"
#include <Robot.h>
#include <OISubsystem.h>
#include <MessageLogger.h>


using namespace xero::misc ;

namespace xero {
    namespace base {
        TeleopController::TeleopController(Robot &robot) : ControllerBase(robot) {
            seq_ = std::make_shared<AutoMode>(robot, "teleop", "Teleop actions") ;
        }

        TeleopController::~TeleopController() {            
        }

        void TeleopController::removeDetector(DetectAutoSequence *detector) {
            std::list<std::shared_ptr<DetectAutoSequence>>::iterator theone ;
            bool found = false ;

            for(auto it = auto_sequences_.begin() ; it != auto_sequences_.end() ; it++) {
                DetectAutoSequence *tryme = (*it).get() ;
                if (tryme == detector) {
                    theone = it ;
                    found = true ;
                }
            }

            if (found)
                auto_sequences_.erase(theone) ;
        }

        void TeleopController::run() {
            MessageLogger &logger = getRobot().getMessageLogger() ;            
            auto oi = getRobot().getOI() ;

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
                //
                // Not running an auto sequence
                //

                //
                // Check for a switch to an auto sequence
                //
                if (running_auto_seq_ == nullptr) {
                    //
                    // If I am currently running teleop, look for something to interrupt
                    // and take over
                    //
                    for(std::shared_ptr<DetectAutoSequence> autoseq: auto_sequences_) {
                        if (autoseq->isTakeoverValid()) {
                            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_ACTIONS_VERBOSE) ;
                            logger << "Teleop: detector fired: " << autoseq->getName() ;
                            logger.endMessage() ;
                            running_auto_seq_ = autoseq->getSequence() ;
                            running_auto_seq_->start() ;
                            oi->getDriverGamepad()->enable(false) ;
                            break ;
                        }
                    }
                }

                if (running_auto_seq_ == nullptr) {
                    //
                    // Still not running an auto sequence
                    //
                    seq_->clear() ;
                    oi->generateActions(seq_) ;
                    seq_->start() ;
                    seq_->run() ;
                    if (!seq_->isDone()) {
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
}