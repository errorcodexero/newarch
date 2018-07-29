#pragma once

#include <memory>

namespace xero {
    namespace sim {
        class SubsystemModel ;
        class RobotSimBase ;

        class Visualizer {
        public:
            Visualizer(RobotSimBase &sim) : simulator_(sim) {            
            }

            virtual ~Visualizer() {                
            }

            RobotSimBase &getSimulator() {
                return simulator_ ;
            }

            virtual void beginCycle(double time) = 0 ;
            virtual void visualizeSubsystem(std::shared_ptr<SubsystemModel> subsystem_p)  = 0 ;
            virtual void endCycle() = 0 ;

        private:
            RobotSimBase &simulator_ ;
        } ;
    }
}
