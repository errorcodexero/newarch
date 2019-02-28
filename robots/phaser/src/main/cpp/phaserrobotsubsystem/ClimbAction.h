#pragma once

#include "PhaserRobotSubsystem.h"
#include <Action.h>

namespace xero{
    namespace phaser{
        class ClimbAction : public xero::base::Action {
        public:
            ClimbAction(PhaserRobotSubsystem &subsystem) ;
            virtual ~ClimbAction() ;

            virtual void start() ;
            virtual void run() ;
            virtual bool isDone() ;
            virtual void cancel() ;
            virtual void abort() ;
            virtual std::string toString() ;

        private:
            enum class State {
                RetractCargoIntake,
                BackupDrivebase,
                ReleaseGrasshopper,
                WaitForDeploy,
                StartWheels,
                Idle,                
            } ;

        private:
            PhaserRobotSubsystem &subsystem_ ;
            State state_ ;
            double delay_start_ ;
            double delay_duration_ ;

            xero::base::ActionPtr retract_cargo_intake_ ;
            xero::base::ActionPtr extend_cargo_intake_ ;     
            xero::base::ActionPtr drive_back_ ;   
            xero::base::ActionPtr deploy_grasshopper_ ;    
            xero::base::ActionPtr drivebase_power_ ;
        };
    }
}