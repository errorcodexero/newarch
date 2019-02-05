#include "DriveByVisionAction.h"
#include "phaserids.h"
#include <cmath>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phaser {
        DriveByVisionAction::DriveByVisionAction(TankDrive &tank_drive, PhaserCameraTracker &camera) : TankDriveAction(tank_drive), camera_(camera)
        {
        }


        void DriveByVisionAction::start() {
            is_done_ = false ;
            getTankDrive().lowGear() ;
        }

        /// \brief Manage the action; called each time through the robot loop
        void DriveByVisionAction::run() {
            MessageLogger &logger = getTankDrive().getRobot().getMessageLogger() ;

            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_VISION_DRIVING) ;
            logger << "DriveByVision:" ;
            logger.endMessage() ; 
        }

        /// \brief Cancel the action
        void DriveByVisionAction::cancel() {            
        }

        /// \brief Return true if the action is complete
        /// \returns True if the action is complete
        bool DriveByVisionAction::isDone() {
            return is_done_ ;
        }

        /// \brief return a human readable string representing the action
        /// \returns a human readable str
        std::string DriveByVisionAction::toString() {
            std::string ret("DriveByVision") ;
            return ret ;
        }
    }
}
