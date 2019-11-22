#include "RanseurCameraTracker.h"
#include "ranseurids.h"
#include <Ranseur.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace ranseur {

        RanseurCameraTracker::RanseurCameraTracker(Subsystem *parent) : LimeLightSubsystem(parent), ITerminator("Vision") {
            threshold_ = parent->getRobot().getSettingsParser().getDouble("ranseurvision:detected_threshold") ;
            area_distance_ratio_ =  parent->getRobot().getSettingsParser().getDouble("ranseurvision:area_distance_ratio") ;
            target_height_ =  parent->getRobot().getSettingsParser().getDouble("ranseurvision:target_height") ;
            camera_height_ =  parent->getRobot().getSettingsParser().getDouble("ranseurvision:camera_height") ;
            camera_angle_ =  parent->getRobot().getSettingsParser().getDouble("ranseurvision:camera_angle") ;
        }

        RanseurCameraTracker::~RanseurCameraTracker() {            
        }

        bool RanseurCameraTracker::canAcceptAction(ActionPtr ptr) {
            if (LimeLightSubsystem::canAcceptAction(ptr))
                return true ;

            return false ;
        }

        void RanseurCameraTracker::computeState() {
            LimeLightSubsystem::computeState() ;

            dist_ = calcDistanceAngles() ;
        }

        double RanseurCameraTracker::calcDistanceArea() {
            //
            // This method calculates the distance to the target based on the
            // readings from the limelight.  This method uses the area of the 
            // target to calculate distance.
            //
            return getTA() * area_distance_ratio_ ;
        }

        double RanseurCameraTracker::calcDistanceAngles() {
            //
            // This method calculates the distance to the target based on the
            // readings from the limelight.  This method uses the Y angle detected
            // to the target to get the distance.  If the camear height is fixed
            // and the target height is fixed, and the angle on the robot of the
            // camera is fixed, this is a simple trig example.
            //
            // See: http://docs.limelightvision.io/en/latest/cs_estimating_distance.html
            //
            return (target_height_ - camera_height_) / tan(camera_angle_ + getTY()) ;
        }        

        bool RanseurCameraTracker::shouldTerminate() {
            if (!isPresent())
            {
                // The limelight is not present, no network table entries
                return false ;
            }

            if (!getTV())
            {
                // No target was detected
                return false ;
            }

            if (dist_ > threshold_)
                return false ;

            return true ;
        }
    }
}