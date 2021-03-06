#pragma once

#include <cameratracker/CameraTracker.h>
#include <frc/Relay.h>
#include <ITerminator.h>

namespace xero {
    namespace phaser {
        class PhaserCameraTracker : public xero::base::CameraTracker, public xero::base::ITerminator
        {
        public:
            PhaserCameraTracker(xero::base::Robot &robot) ;           

            virtual ~PhaserCameraTracker()  ;

            virtual void computeState() ;

            virtual bool shouldTerminate() ;

            virtual double getRectRatio() const {
                return rect_ratio_ ;
            }            

            void setVisionDetectThreshold(double d) {
                distance_threshold_ = d ;
            }

            virtual bool canAcceptAction(xero::base::ActionPtr act) ;

        private:
            constexpr static const char *TargetRectRatio = "rect_ratio" ;

        private:
            double distance_threshold_ ;
            double rect_ratio_min_ ;
            double rect_ratio_max_ ;         
            double rect_ratio_ ;   
        } ;
    }
}