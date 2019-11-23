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
            double getYaw(){
                return getTX() ;
            }
            virtual bool canAcceptAction(xero::base::ActionPtr act) ;

        private:

        private:
            double distance_ ;
            double distance_threshold_ ;
            double camera_angle_ ;
            double camera_height_ ;
            double target_height_ ;
        } ;
    }
}