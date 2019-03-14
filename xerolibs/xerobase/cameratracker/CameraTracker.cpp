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
            relay_state_ = frc::Relay::Value::kOff ;      

            camera_ = -1 ;
            mode_ = CameraMode::Invalid ;
        }

        CameraTracker::~CameraTracker()
        {            
        }

        void CameraTracker::computeState()
        {            
            is_valid_ = table_->GetBoolean(TargetDetected, false) ;
            if (is_valid_) {
                dist_inch_ = table_->GetNumber(TargetDistance, 0.0) * 0.71 ;
                yaw_deg_ = table_->GetNumber(TargetAngle, 0.0) ;
            }

            bool is_enabled = getRobot().IsEnabled() ;


            MessageLogger &logger = getRobot().getMessageLogger() ;
            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_CAMERA_TRACKER_VERBOSE) ;
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
                case CameraMode::Invalid:
                    break ;
                }
            }

            setLEDRing() ;
        }

        std::string CameraTracker::toString(frc::Relay::Value v) {
            std::string ret = "???" ;
            switch(v) {
                case frc::Relay::Value::kForward:
                    ret = "forward" ;
                    break ;
                case frc::Relay::Value::kReverse:
                    ret = "reverse" ;
                    break ;
                case frc::Relay::Value::kOn:
                    ret = "on" ;
                    break ;
                case frc::Relay::Value::kOff:
                    ret = "off" ;
                    break ;
            }

            return ret ;
        }

        void CameraTracker::setLEDRing() {
            frc::Relay::Value newvalue = frc::Relay::Value::kOff ;
            if (mode_ == CameraMode::DriverViewing)
                newvalue = frc::Relay::Value::kOff ;
            else {
                if (camera_ == 0)
                    newvalue = frc::Relay::Value::kReverse ;
                else
                    newvalue = frc::Relay::Value::kForward ;
            }      

            if (newvalue != relay_state_) {
                relay_->Set(newvalue) ;

                MessageLogger &logger = getRobot().getMessageLogger() ;
                logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_CAMERA_TRACKER) ;
                logger << "Camera: changing LED ring state" ;
                logger << ", old state " << toString(relay_state_) ;
                logger << ", new state " << toString(newvalue) ;
                logger.endMessage() ;

                relay_state_ = newvalue ;                
            }
        }
    }
}