#pragma once

#include "PhaserRobotSubsystem.h"
#include <Action.h>

namespace xero{
    namespace phaser{
        class StrafeAction : public xero::base::Action {
        public:
            StrafeAction(PhaserRobotSubsystem &subsystem, bool reverse, int count) ;
            virtual ~StrafeAction() ;

            virtual void start() ;
            virtual void run() ;
            virtual bool isDone() ;
            virtual void cancel() ;
            virtual void abort() ;
            virtual std::string toString() ;

        private:
            enum class State {
                SearchingForLine,
                ToShootPosition,
                Shoot,
                Idle,
            } ;

        private:
            PhaserRobotSubsystem &subsystem_ ;
            State state_ ;
            int count_ ;
            bool reverse_ ;
            bool detected_ ;
            int thiscount ;
            double trigger_dist_ ;
            double shoot_dist_ ;

            xero::base::ActionPtr drive_straight_ ;
            xero::base::ActionPtr drive_stop_ ;
            xero::base::ActionPtr shoot_ ;
        };
    }
}