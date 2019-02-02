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
            void setCameraMode(CameraMode mode) ;

        private:
            static const char *NetworkTableName ;
            static const char *TargetDetected ;
            static const char *TargetDistance ;
            static const char *TargetAngle ;
            static const char *TargetRectRatio ;
            static const char *CameraNumber ;
            static const char *CameraModeName ;

        private:
            std::shared_ptr<nt::NetworkTable> table_ ;
            bool is_valid_ ;
            double dist_inch_ ;
            double yaw_deg_ ;
            double rect_ratio_ ;
            size_t camera_ ;
            CameraMode mode_ ;
        } ;
    }
}