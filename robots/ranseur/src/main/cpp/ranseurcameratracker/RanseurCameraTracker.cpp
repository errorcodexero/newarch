#include "RanseurCameraTracker.h"
#include "ranseurids.h"
#include "SetThresholdAction.h"
#include <Ranseur.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace ranseur {

        RanseurCameraTracker::RanseurCameraTracker(Robot &robot) : CameraTracker(robot), ITerminator("Vision") {
            distance_threshold_ = robot.getSettingsParser().getDouble("cameratracker:distance_threshold") ;
            rect_ratio_min_ = robot.getSettingsParser().getDouble("cameratracker:rect_ratio_min") ;
            rect_ratio_max_ = robot.getSettingsParser().getDouble("cameratracker:rect_ratio_max") ;      
        }

        RanseurCameraTracker::~RanseurCameraTracker() {            
        }

        bool RanseurCameraTracker::canAcceptAction(ActionPtr ptr) {
            if (CameraTracker::canAcceptAction(ptr))
                return true ;

            auto act = std::dynamic_pointer_cast<SetThresholdAction>(ptr) ;
            return act != nullptr ;
        }

        void RanseurCameraTracker::computeState() {
            rect_ratio_ = getNetworkTable()->GetNumber(TargetRectRatio, 0.0) ;
            CameraTracker::computeState() ;
        }

        bool RanseurCameraTracker::shouldTerminate() {
            if (!isValid()) {
                MessageLogger &logger = getRobot().getMessageLogger() ;
                logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_VISION_TERMINATOR) ;
                logger << "RanseurCameraTracker: did not detect target" ;
                logger.endMessage() ;                  
                return false ;
            }
            
            MessageLogger &logger = getRobot().getMessageLogger() ;
            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_VISION_TERMINATOR) ;
            logger << "RanseurCameraTracker: distance " << getDistance() ;
            logger.endMessage() ;  

            if (getDistance() > distance_threshold_)
                return false ;

            return true ;
        }
    }
}