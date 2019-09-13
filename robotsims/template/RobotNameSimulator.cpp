#include "$$RobotName$$Simulator.h"
#include "$$RobotName$$ScreenVisualizer.h"
#include "OIModel.h"
#include <TankDriveModel.h>
#include <cassert>
#include <iostream>
#include <cmath>

using namespace ctre::phoenix::motorcontrol::can ;
using namespace frc ;
using namespace xero::sim ;

namespace xero
{
    namespace sim
    {
        namespace $$robotname$$
        {
            $$RobotName$$Simulator::$$RobotName$$Simulator(const std::string &paramfile) : RobotSimBase(paramfile)
            {
                tankdrive_ = std::make_shared<TankDriveModel>(*this) ;
                addModel(tankdrive_) ;

                oi_ = std::make_shared<OIModel>(*this) ;
                addModel(oi_) ;      

                visualizer_ = false ;
            }

            $$RobotName$$Simulator::~$$RobotName$$Simulator() {
            }

            void $$RobotName$$Simulator::enableScreen() {
                visualizer_ = true ;
            }

            void $$RobotName$$Simulator::connect(SimulatedObject *device) {
                if (visualizer_) {
                    auto vis = std::make_shared<$$RobotName$$ScreenVisualizer>(*this) ;
                    addVisualizer(vis) ;                    
                    visualizer_ = false ;
                }

                RobotSimBase::connect(device) ;
            }

            void $$RobotName$$Simulator::disconnect(SimulatedObject *device) {
                RobotSimBase::disconnect(device) ;
            }
        }
    }
}
