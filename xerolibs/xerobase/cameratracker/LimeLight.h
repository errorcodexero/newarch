#pragma once

#include "Subsystem.h"
#include <networktables/NetworkTable.h>
#include <frc/Relay.h>

namespace xero {
    namespace base {
        class LimeLight : public Subsystem
        {
        public:
            enum class camMode
            {
                VisionProcessor,
                DriverCamera,
                Invalid,
            } ;

            enum class ledMode
            {
                UseLED,
                ForceOff,
                ForceBlink,
                ForceOn,
                Invalid,

            } ;

        public:
            LimeLight(Subsystem *parent) ;
            virtual ~LimeLight() ;

            virtual void computeState() ;
            virtual void run() ;
            virtual bool canAcceptAction(ActionPtr action) ;

            void setCamMode(camMode mode) ;
            void setLedMode(ledMode mode) ;
            bool isLimeLightPresent(){
                return present_ ;
            }
            bool isTargetPresent(){
                return tv_ ;
            }
            double getTX(){
                return tx_ ;
            }
            double getTY(){
                return ty_ ;
            }
            double getTA(){
                return ta_ ;
            }

            camMode getCamMode() {
                return cam_mode_ ;
            }

            ledMode getLEDMode(){
                return led_mode_ ;
            }

            static std::string toString();

            

        protected:
            std::shared_ptr<nt::NetworkTable> getNetworkTable() {
                return table_ ;
            }

        private:
            constexpr static const char *camModeName = "camera_mode" ;
            constexpr static const char *ledModeName = "led_mode" ;
            

        private:
            std::shared_ptr<nt::NetworkTable> table_ ;
            camMode cam_mode_ ;
            ledMode led_mode_ ;

            double ta_ ;
            bool tv_ ;
            double tx_ ; 
            double ty_ ;
            bool present_ ;
        } ;
    }
}