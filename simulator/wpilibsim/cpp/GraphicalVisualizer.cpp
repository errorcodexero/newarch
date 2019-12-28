#include <frc/GraphicalVisualizer.h>
#include <frc/RobotSimBase.h>
#include <cassert>
#include <string>
#include <cmath>
#include <algorithm>
#include <iostream>

using namespace xero::misc ;

namespace xero {
    namespace sim {

        GraphicalVisualizer::GraphicalVisualizer(RobotSimBase &sim) : Visualizer(sim) {    
        }

        GraphicalVisualizer::~GraphicalVisualizer() {
        }

        void GraphicalVisualizer::beginCycle(double t) {
            MessageLogger &logger = getSimulator().getRobotMessageLogger() ;
            logger.startMessage(MessageLogger::MessageType::info) ;
            logger << "%%%%system,time:" << t ;
            logger.endMessage() ;
        }

        void GraphicalVisualizer::endCycle() {
        }

        void GraphicalVisualizer::visualizeSubsystem(std::shared_ptr<SubsystemModel> model_p)
        {
            MessageLogger &logger = getSimulator().getRobotMessageLogger() ;            
            std::list<std::string> lines ;
            model_p->generateDisplayInformation(lines) ;

            for(const std::string &line : lines)
            {
                logger.startMessage(MessageLogger::MessageType::info) ;
                logger << "%%%%" << model_p->getName() << "," << line ;
                logger.endMessage() ;  
            }
        }
    }
}
