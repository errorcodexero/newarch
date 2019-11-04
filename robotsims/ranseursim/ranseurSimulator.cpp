#include "ranseurSimulator.h"
#include "ranseurScreenVisualizer.h"

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
        namespace ranseur
        {
            ranseurSimulator::ranseurSimulator(const std::string &paramfile) : RobotSimBase(paramfile)
            {
                tankdrive_ = std::make_shared<TankDriveModel>(*this) ;
                addModel(tankdrive_) ;

                oi_ = std::make_shared<OIModel>(*this) ;
                addModel(oi_) ;      

                tub_manipulator_ = std::make_shared<TubManipulatorModel>(*this) ;
                addModel(tub_manipulator_) ;

                bunny_arm_ = std::make_shared<BunnyArmModel>(*this) ;
                addModel(bunny_arm_) ;

                visualizer_ = false ;
            }

            ranseurSimulator::~ranseurSimulator() {
            }

            void ranseurSimulator::enableScreen() {
                visualizer_ = true ;
            }

            void ranseurSimulator::connect(SimulatedObject *device) {
                if (visualizer_) {
                    auto vis = std::make_shared<ranseurScreenVisualizer>(*this) ;
                    addVisualizer(vis) ;                    
                    visualizer_ = false ;
                }

                RobotSimBase::connect(device) ;
            }

            void ranseurSimulator::disconnect(SimulatedObject *device) {
                RobotSimBase::disconnect(device) ;
            }
        }
    }
}
