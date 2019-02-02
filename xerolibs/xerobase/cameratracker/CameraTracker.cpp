#include "CameraTracker.h"
#include "Robot.h"
#include "basegroups.h"
#include <networktables/NetworkTableInstance.h>
#include <iostream>

using namespace xero::misc ;

namespace xero {
    namespace base {
        const char *CameraTracker::NetworkTableName = "TargetTracking" ;
        const char *CameraTracker::TargetDetected = "valid" ;
        const char *CameraTracker::TargetDistance = "dist_inch" ;
        const char *CameraTracker::TargetAngle = "yaw_deg" ;
        const char *CameraTracker::TargetRectRatio = "rect_ration" ;
        const char *CameraTracker::CameraNumber = "camera_number" ;
        const char *CameraTracker::CameraModeName = "camera_mode" ;

        CameraTracker::CameraTracker(Robot &robot) : Subsystem(robot, "CameraTracker")
        {
            nt::NetworkTableInstance ntinst = nt::NetworkTableInstance::GetDefault() ;
            table_ = ntinst.GetTable(NetworkTableName) ;
        }

        CameraTracker::~CameraTracker()
        {            
        }

        void CameraTracker::computeState()
        {            
            is_valid_ = table_->GetBoolean(TargetDetected, false) ;
            if (is_valid_) {
                dist_inch_ = table_->GetNumber(TargetDistance, 0.0) ;
                yaw_deg_ = table_->GetNumber(TargetAngle, 0.0) ;
                rect_ratio_ = table_->GetNumber(TargetRectRatio, 0.0) ;
            }

            MessageLogger &logger = getRobot().getMessageLogger() ;
            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_CAMERA_TRACKER) ;
            logger << "CameraTracker" ;
            logger << " is_valid " << is_valid_ ;
            logger << " dist_inch " << dist_inch_ ;
            logger << " yaw_deg " << yaw_deg_ ;
            logger << " rect_ration " << rect_ratio_ ;
            logger.endMessage() ;
        }


        void CameraTracker::run()
        {            
        }

        bool CameraTracker::canAcceptAction(ActionPtr action)
        {            
            return false ;
        }

        void CameraTracker::setCameraIndex(size_t which)
        {
            if (which != camera_)
            {
                camera_ = which ;
                table_->PutNumber(CameraNumber, which) ;
            }
        }

        void CameraTracker::setCameraMode(CameraMode mode)
        {
            if (mode_ != mode)
            {
                mode_ = mode ;

                switch(mode)
                {
                case CameraMode::TargetTracking:
                    table_->PutString(CameraModeName, "TargetTracking") ;
                    break ;
                case CameraMode::DriverViewing:
                    table_->PutString(CameraModeName, "DriverViewing") ;                
                    break ;
                }
            }
        }
    }
}