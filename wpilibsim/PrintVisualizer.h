#pragma once

#include "Visualizer.h"
#include <iostream>

namespace xero {
    namespace sim {
        class PrintVisualizer : public Visualizer {
        public:
            PrintVisualizer(std::ostream &strm) ;
            virtual ~PrintVisualizer() ;

            virtual void beginCycle(double time) ;
            virtual void visualizeSubsystem(std::shared_ptr<SubsystemModel> subsystem_p)  ;
            virtual void endCycle() ;

        private:
            std::ostream &stream_ ;
        } ;
    }
}