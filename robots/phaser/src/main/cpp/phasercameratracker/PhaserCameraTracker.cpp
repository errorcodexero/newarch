#include "PhaserCameraTracker.h"
#include <Robot.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phaser {

        PhaserCameraTracker::PhaserCameraTracker(Robot &robot) : CameraTracker(robot) {
            distance_threshold_ = robot.getSettingsParser().getDouble("cameratracker:distance_threshold") ;
            yaw_threshold_ = robot.getSettingsParser().getDouble("cameratracker:yaw_threshold") ;
            rect_ratio_min_ = robot.getSettingsParser().getDouble("cameratracker:rect_ratio_min") ;
            rect_ratio_max_ = robot.getSettingsParser().getDouble("cameratracker:rect_ratio_max") ;      

            light_ring_ = std::make_shared<frc::Relay>(0) ;

        }

        PhaserCameraTracker::~PhaserCameraTracker() {            
        }

        void PhaserCameraTracker::computeState() {
            rect_ratio_ = getNetworkTable()->GetNumber(TargetRectRatio, 0.0) ;
            CameraTracker::computeState() ;

            light_ring_->Set(frc::Relay::Value::kReverse) ;            
        }

        bool PhaserCameraTracker::shouldTerminate() {
            if (!isValid())
                return false ;

            if (getDistance() > distance_threshold_)
                return false ;

            return getRectRatio() > rect_ratio_min_ && getRectRatio() < rect_ratio_max_ ;
        }
    }
}