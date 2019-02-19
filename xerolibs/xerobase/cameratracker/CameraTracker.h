#pragma once

#include "Subsystem.h"
#include <networktables/NetworkTable.h>

namespace xero {
    namespace base {
        class CameraTracker : public Subsystem
        {
        public:
            enum class CameraMode
            {
                TargetTracking,
                DriverViewing,
            } ;

        public:
            CameraTracker(Robot &robot) ;
            virtual ~CameraTracker() ;

            virtual void computeState() ;
            virtual void run() ;
            virtual bool canAcceptAction(ActionPtr action) ;

            void setCameraIndex(size_t which)  ;
            size_t getCameraIndex() {
                return camera_ ;
            }

            void setCameraMode(CameraMode mode) ;

            CameraMode getCameraMode() {
                return mode_ ;
            }

            bool isValid() const {
                return is_valid_ ;
            }

            virtual double getDistance() const {
                return dist_inch_ ;
            }

            virtual double getYaw() const {
                return yaw_deg_ ;
            }

        protected:
            std::shared_ptr<nt::NetworkTable> getNetworkTable() {
                return table_ ;
            }

        private:
            constexpr static const char *NetworkTableName = "TargetTracking" ;
            constexpr static const char *TargetDetected = "valid" ;
            constexpr static const char *TargetDistance = "dist_inch" ;
            constexpr static const char *TargetAngle = "yaw_deg" ;
            constexpr static const char *CameraNumber ="camera_number";
            constexpr static const char *CameraModeName = "camera_mode" ;

        private:
            std::shared_ptr<nt::NetworkTable> table_ ;
            bool is_valid_ ;
            double dist_inch_ ;
            double yaw_deg_ ;
            size_t camera_ ;
            CameraMode mode_ ;
        } ;
    }
}