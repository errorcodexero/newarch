#pragma once

#include <frc/Visualizer.h>

namespace xero {
    namespace sim {
        class GraphicalVisualizer : public Visualizer {
        public:
            GraphicalVisualizer(RobotSimBase &sim) ;
            virtual ~GraphicalVisualizer() ;

            virtual void beginCycle(double time) ;
            virtual void endCycle() ;       

            void visualizeSubsystem(std::shared_ptr<SubsystemModel> model_p) ;

        protected:
        } ;
    }
}
