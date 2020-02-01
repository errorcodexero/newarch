#pragma once

#include <cameratracker/LimeLight.h>

namespace xero {
    namespace droid {
        class DroidLimeLight : public xero::base::LimeLight
        {
        public:
            DroidLimeLight(xero::base::Subsystem *parent) ;           

            virtual ~DroidLimeLight()  ;

            virtual void computeState() ; 

            double getDistance(){
                return distance_ ;
            }   
            double getYaw() {
                return yaw_ ;
            }

            double getSampleTime() { return sample_time_; }

            virtual void init(xero::base::LoopType lt) ;
            virtual void reset() ;

        private:
            double yaw_ ;
            double distance_ ;
            double sample_time_;
            double camera_angle_ ;
            double camera_height_ ;
            double target_height_ ;
            int plotid_ ;
        } ;
    }
}