#pragma once

#include "RobotSimBase.h"
#include "SampleRobot.h"
#include <memory>

namespace xero {
    namespace sim {
        class SubsystemModel ;

        class Visualizer {
        public:
            Visualizer(RobotSimBase &sim) : simulator_(sim) {            
            }

            virtual ~Visualizer() {                
            }

            RobotSimBase &getSimulator() {
                return simulator_ ;
            }

            std::string getModeString() {
                std::string str("Finished") ;

                if (getSimulator().getRobot()->IsDisabled())
                    str = "Disabled" ;
                else if (getSimulator().getRobot()->IsAutonomous())
                    str = "Autonomous" ;
                else if (getSimulator().getRobot()->IsOperatorControl())
                    str = "Teleop" ;
                else if (getSimulator().getRobot()->IsTest())
                    str = "Test" ;              

                return str ;
            }            

            virtual void beginCycle(double time) = 0 ;
            virtual void visualizeSubsystem(std::shared_ptr<SubsystemModel> subsystem_p)  = 0 ;
            virtual void endCycle() = 0 ;

        private:
            RobotSimBase &simulator_ ;
        } ;
    }
}
