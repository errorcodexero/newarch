#pragma once

#include "Subsystem.h"
#include <networktables/NetworkTable.h>
#include <frc/Relay.h>

namespace xero {
    namespace base {
        class LimeLightSubsystem : public Subsystem
        {
            friend class LimeLightChangeModeAction ;

        public:
            enum class CameraMode
            {
                TargetTracking,
                DriverViewing,
                Invalid,
            } ;

            enum class LEDMode
            {
                Pipeline,
                Off,
                Blink,
                On,
                Invalid
            } ;

        public:
            LimeLightSubsystem(Subsystem *parent) ;
            virtual ~LimeLightSubsystem() ;

            virtual void computeState() ;
            virtual void run() ;
            virtual bool canAcceptAction(ActionPtr action) ;

            void setMode(CameraMode mode) ;
            void setLED(LEDMode mode) ;
            void setPipeline(int pipeline) ;

            bool getTV() {
                return tv_ ;
            }

            double getTX() {
                return tx_ ;
            }

            double getTY() {
                return ty_ ;
            }

            double getTA() {
                return ta_ ;
            }

            bool isPresent() {
                return present_ ;
            }

            CameraMode getMode() {
                return mode_ ;
            }

            LEDMode getLED() {
                return led_ ;
            }

            int getPipeline() {
                return pipeline_ ;
            }

            static std::string toString(CameraMode mode) {
                std::string ret = "????" ;

                switch(mode) {
                    case CameraMode::DriverViewing:
                        ret = "DriverViewing" ;
                        break ;
                    case CameraMode::TargetTracking:
                        ret = "TargetTracking" ;
                        break ;
                    case CameraMode::Invalid:
                        ret = "Invalid" ;
                        break ;
                }

                return ret ;
            }

            static std::string toString(LEDMode mode) {
                std::string ret = "????" ;

                switch(mode) {
                    case LEDMode::Pipeline:
                        ret = "Pipeline" ;
                        break ;
                    case LEDMode::On:
                        ret = "On" ;
                        break ;
                    case LEDMode::Off:
                        ret = "Off" ;
                        break ;
                    case LEDMode::Blink:
                        ret = "Blink" ;
                        break ;                                                                        
                    case LEDMode::Invalid:
                        ret = "Invalid" ;
                        break ;
                }

                return ret ;
            }            

        protected:
            std::shared_ptr<nt::NetworkTable> getNetworkTable() {
                return table_ ;
            }


        private:
            constexpr static const char *NetworkTableName = "limelight" ;
            constexpr static const char *TV = "tv" ;
            constexpr static const char *TX = "tx" ;
            constexpr static const char *TY = "ty" ;
            constexpr static const char *TA = "ta" ;
            constexpr static const char *LEDMODE ="ledMode";
            constexpr static const char *CAMMODE = "camMode" ;
            constexpr static const char *PIPELINE = "pipeline" ;

        private:
            std::shared_ptr<nt::NetworkTable> table_ ;
            CameraMode mode_ ;
            LEDMode led_ ;
            int pipeline_ ;
            bool present_ ;

            bool tv_ ;
            double tx_ ;
            double ty_ ;
            double ta_ ;
        } ;
    }
}