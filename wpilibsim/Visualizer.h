#pragma once

#include <memory>

namespace xero {
    namespace sim {
        class SubsystemModel ;

        class Visualizer {
        public:
            Visualizer() {                
            }

            virtual ~Visualizer() {                
            }

            virtual void beginCycle(double time) = 0 ;
            virtual void visualizeSubsystem(std::shared_ptr<SubsystemModel> subsystem_p)  = 0 ;
            virtual void endCycle() = 0 ;
        } ;
    }
}