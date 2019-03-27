#pragma once 

#include "TurntableAction.h"
#include "Turntable.h"
#include "phasercameratracker/PhaserCameraTracker.h"

namespace xero{
    namespace phaser{
        class CargoTrackerAction : public TurntableAction {
        public:
            CargoTrackerAction(Turntable &subsystem, PhaserCameraTracker &vision) ;
            virtual ~CargoTrackerAction() ;

            virtual void start() ;
            virtual void run() ;
            virtual bool isDone() ;
            virtual void cancel() ;
            virtual std::string toString() ;

        private:
            PhaserCameraTracker &vision_ ;
            double kp_ ;
        } ;
    }
}