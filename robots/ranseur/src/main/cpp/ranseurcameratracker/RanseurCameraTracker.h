#pragma once

#include <cameratracker/LimeLightSubsystem.h>
#include <frc/Relay.h>
#include <ITerminator.h>

namespace xero {
    namespace ranseur {
        class RanseurCameraTracker : public xero::base::LimeLightSubsystem, public xero::base::ITerminator
        {
        public:
            RanseurCameraTracker(xero::base::Subsystem *parent) ;           

            double getDistance() {
                return dist_ ;
            }

            virtual ~RanseurCameraTracker()  ;

            virtual void computeState() ;

            virtual bool shouldTerminate() ;

            virtual bool canAcceptAction(xero::base::ActionPtr act) ;

        private:
            double calcDistanceArea() ;
            double calcDistanceAngles() ;

        private:
            // The distance to the target
            double dist_ ;

            // The threshold for detecting a tub
            double threshold_ ;

            // Distance method one
            double area_distance_ratio_ ;

            // Distance method two
            double target_height_ ;         // Fixed height of target from floor
            double camera_height_ ;         // Fixed height of camera from floor
            double camera_angle_ ;          // Angle of the camera on the robot (should be zero)
        } ;
    }
}