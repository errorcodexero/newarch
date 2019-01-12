#include "PrintVisualizer.h"
#include "SubsystemModel.h"

namespace xero {
    namespace sim {
        PrintVisualizer::PrintVisualizer(RobotSimBase &sim, std::ostream &strm) : Visualizer(sim), stream_(strm) {
        }

        PrintVisualizer::~PrintVisualizer() {            
        }

        void PrintVisualizer::beginCycle(double time) {
            stream_ << "Time: " << time  << std::endl; 
        }

        void PrintVisualizer::visualizeSubsystem(std::shared_ptr<SubsystemModel> model) {
            stream_ << "    " << model->toString() << std::endl ;
        }

        void PrintVisualizer::endCycle() {
            stream_ << std::endl << std::endl ;
        }
    }
}