#include "PhaserCameraTracker.h"
#include "phaserids.h"
#include "SetThresholdAction.h"
#include <Robot.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phaser {

        PhaserCameraTracker::PhaserCameraTracker(Robot &robot) : CameraTracker(robot), ITerminator("Vision") {
            distance_threshold_ = robot.getSettingsParser().getDouble("cameratracker:distance_threshold") ;
            rect_ratio_min_ = robot.getSettingsParser().getDouble("cameratracker:rect_ratio_min") ;
            rect_ratio_max_ = robot.getSettingsParser().getDouble("cameratracker:rect_ratio_max") ;      
        }

        PhaserCameraTracker::~PhaserCameraTracker() {            
        }

        bool PhaserCameraTracker::canAcceptAction(ActionPtr ptr) {
            if (CameraTracker::canAcceptAction(ptr))
                return true ;

            auto act = std::dynamic_pointer_cast<SetThresholdAction>(ptr) ;
            return act != nullptr ;
        }

        void PhaserCameraTracker::computeState() {
            rect_ratio_ = getNetworkTable()->GetNumber(TargetRectRatio, 0.0) ;
            CameraTracker::computeState() ;
        }

        bool PhaserCameraTracker::shouldTerminate() {
            if (!isValid()) {
                MessageLogger &logger = getRobot().getMessageLogger() ;
                logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_VISION_TERMINATOR) ;
                logger << "PhaserCameraTracker: did not detect target" ;
                logger.endMessage() ;                  
                return false ;
            }

            MessageLogger &logger = getRobot().getMessageLogger() ;
            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_VISION_TERMINATOR) ;
            logger << "PhaserCameraTracker: distance " << getDistance() ;
            logger.endMessage() ;  

            if (getDistance() > distance_threshold_)
                return false ;

            return true ;
        }
    }
}