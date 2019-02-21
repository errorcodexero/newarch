#include "CameraTracker.h"
#include "CameraChangeAction.h"
#include "Robot.h"
#include "basegroups.h"
#include <networktables/NetworkTableInstance.h>
#include <iostream>

using namespace xero::misc ;

namespace xero {
    namespace base {
        CameraTracker::CameraTracker(Robot &robot) : Subsystem(robot, "CameraTracker")
        {
            nt::NetworkTableInstance ntinst = nt::NetworkTableInstance::GetDefault() ;
            table_ = ntinst.GetTable(NetworkTableName) ;

            relay_ = std::make_shared<frc::Relay>(0) ;
            relay_->Set(frc::Relay::Value::kOff) ;               
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
            }

            bool is_enabled = getRobot().IsEnabled() ;

            MessageLogger &logger = getRobot().getMessageLogger() ;
            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_CAMERA_TRACKER) ;
            logger << "CameraTracker" ;
            logger << " is_valid " << is_valid_ ;
            logger << " enabled " << is_enabled ;
            logger << " dist_inch " << dist_inch_ ;
            logger << " yaw_deg " << yaw_deg_ ;
            logger.endMessage() ;
        }

        void CameraTracker::run()
        {            
        }

        bool CameraTracker::canAcceptAction(ActionPtr action)
        {            
            std::shared_ptr<CameraChangeAction> camact = std::dynamic_pointer_cast<CameraChangeAction>(action) ;
            return camact != nullptr ;
        }

        void CameraTracker::setCameraIndex(size_t which)
        {
            if (which != camera_)
            {
                camera_ = which ;
                table_->PutNumber(CameraNumber, which) ;
            }
            setLEDRing() ;
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

            setLEDRing() ;
        }

        void CameraTracker::setLEDRing() {
            if (mode_ == CameraMode::DriverViewing)
                relay_->Set(frc::Relay::Value::kOff) ;
            else {
                if (camera_ == 0)
                    relay_->Set(frc::Relay::Value::kReverse) ;
                else
                    relay_->Set(frc::Relay::Value::kForward) ;
            }      
        }
    }
}