#pragma once

#include "PhaserRobotSubsystem.h"
#include <Action.h>
#include <list>

namespace xero{
    namespace phaser{
        class ClimbAction : public xero::base::Action {
        public:
            ClimbAction(PhaserRobotSubsystem &subsystem, bool complete) ;
            virtual ~ClimbAction() ;

            virtual void start() ;
            virtual void run() ;
            virtual bool isDone() ;
            virtual void cancel() ;
            virtual void abort() ;
            virtual std::string toString() ;

        private:
            enum class State {
                ReleaseGrasshopper,
                WaitForDeploy,
                StartWheels,
                DeployCargoCollector,
                WaitForStopped,
                Backup,
                Idle              
            } ;

        private:
            PhaserRobotSubsystem &subsystem_ ;
            State state_ ;
            double start_ ;
            double deploy_delay_ ;
            double backup_delay_ ;

            xero::base::ActionPtr extend_cargo_intake_ ;   
            xero::base::ActionPtr retract_cargo_intake_ ;               
            xero::base::ActionPtr drive_back_ ;   
            xero::base::ActionPtr deploy_grasshopper_ ;    
            xero::base::ActionPtr drivebase_power_ ;

            std::list<double> velocities_ ;
            double velocity_still_threshold_ ;

            size_t velocity_sample_count_ ;

            bool complete_ ;
        };
    }
}