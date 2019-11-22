#include "RanseurSimulator.h"
#include "RanseurScreenVisualizer.h"

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
            RanseurSimulator::RanseurSimulator(const std::string &paramfile) : RobotSimBase(paramfile)
            {
                tankdrive_ = std::make_shared<TankDriveModel>(*this, TankDriveModel::MotorType::MTSpark) ;
                addModel(tankdrive_) ;

                oi_ = std::make_shared<OIModel>(*this) ;
                addModel(oi_) ;      

                tub_manipulator_ = std::make_shared<TubManipulatorModel>(*this) ;
                addModel(tub_manipulator_) ;

                bunny_arm_ = std::make_shared<BunnyArmModel>(*this) ;
                addModel(bunny_arm_) ;

                vision_ = std::make_shared<RanseurVision>(*this, *tankdrive_) ;
                addModel(vision_) ;

                visualizer_ = false ;
            }

            RanseurSimulator::~RanseurSimulator() {
            }

            void RanseurSimulator::enableScreen() {
                visualizer_ = true ;
            }

            void RanseurSimulator::connect(SimulatedObject *device) {
                if (visualizer_) {
                    auto vis = std::make_shared<RanseurScreenVisualizer>(*this) ;
                    addVisualizer(vis) ;                    
                    visualizer_ = false ;
                }

                RobotSimBase::connect(device) ;
            }

            void RanseurSimulator::disconnect(SimulatedObject *device) {
                RobotSimBase::disconnect(device) ;
            }
        }
    }
}
