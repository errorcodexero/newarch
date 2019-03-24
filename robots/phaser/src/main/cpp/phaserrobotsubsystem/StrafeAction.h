#pragma once

#include "PhaserRobotSubsystem.h"
#include <Action.h>

namespace xero{
    namespace phaser{
        class StrafeAction : public xero::base::Action {
        public:
            // Action for the cargo ship
            StrafeAction(PhaserRobotSubsystem &subsystem, int count) ;

            // Action for the rocket
            StrafeAction(PhaserRobotSubsystem &subsystem) ;
            virtual ~StrafeAction() ;

            virtual void start() ;
            virtual void run() ;
            virtual bool isDone() ;
            virtual void cancel() ;
            virtual void abort() ;
            virtual std::string toString() ;

        private:
            bool isLineDetectedFront() ;
            bool isLineDetectedBack() ;            

        private:
            enum class State {
                Idle,                
                ArmedShip,
                ArmedRocket,
                WaitForDistanceForward,
                WaitForDistanceBack,
                Shooting
            } ;

        private:
            PhaserRobotSubsystem &subsystem_ ;
            State state_ ;
            int count_ ;
            bool front_detected_ ;
            bool back_detected_ ;
            int thiscount_ ;
            double shoot_dist_ ;
            double start_dist_ ;

            double vel_factor_ ;
            double target_dist_ ;
            bool ship_ ;
            bool forward_ ;

            xero::base::ActionPtr rumble_ ;
            xero::base::ActionPtr shoot_ ;
        };
    }
}