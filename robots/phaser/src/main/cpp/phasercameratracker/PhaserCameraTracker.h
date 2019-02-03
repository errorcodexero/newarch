#pragma once

#include <cameratracker/CameraTracker.h>
#include <ITerminator.h>

namespace xero {
    namespace phaser {
        class PhaserCameraTracker : public xero::base::CameraTracker, public xero::base::ITerminator
        {
        public:
            PhaserCameraTracker(xero::base::Robot &robot) ;           

            virtual ~PhaserCameraTracker()  ;

            virtual bool shouldTerminate() ;

        private:
            double distance_threshold_ ;
            double yaw_threshold_ ;
            double rect_ratio_min_ ;
            double rect_ratio_max_ ;            
        } ;
    }
}