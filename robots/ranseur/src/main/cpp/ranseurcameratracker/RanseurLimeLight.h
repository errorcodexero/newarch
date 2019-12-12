#pragma once

#include <cameratracker/LimeLight.h>
#include <frc/Relay.h>
#include <ITerminator.h>

namespace xero {
    namespace ranseur {
        class RanseurLimeLight : public xero::base::LimeLight, public xero::base::ITerminator
        {
        public:
            RanseurLimeLight(xero::base::Subsystem *parent) ;           

            virtual ~RanseurLimeLight()  ;

            virtual void computeState() ;

            virtual bool shouldTerminate() ;   

            double getDistance(){
                return distance_ ;
            }   
            double getYaw() {
                return yaw_ ;
            }

            virtual bool canAcceptAction(xero::base::ActionPtr act) ;

            virtual void init(xero::base::LoopType lt) ;
            virtual void reset() ;

        private:

        private:
            double prev_dist_ ;
            int count_ ;
            int target_count_ ;
            double yaw_ ;
            double dist_angle_ ;
            double dist_area_constant_ ;
            double distance_ ;
            double distance_threshold_ ;
            double camera_angle_ ;
            double camera_height_ ;
            double target_height_ ;
            int plotid_ ;
        } ;
    }
}