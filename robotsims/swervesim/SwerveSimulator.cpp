#include "SwerveSimulator.h"
#include "SwerveScreenVisualizer.h"
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
        namespace swerve
        {
            SwerveSimulator::SwerveSimulator(const std::string &paramfile) : RobotSimBase(paramfile)
            {
                tankdrive_ = std::make_shared<TankDriveModel>(*this) ;
                addModel(tankdrive_) ;

                oi_ = std::make_shared<OIModel>(*this) ;
                addModel(oi_) ;      

                visualizer_ = false ;
            }

            SwerveSimulator::~SwerveSimulator() {
            }

            void SwerveSimulator::enableScreen() {
                visualizer_ = true ;
            }

            void SwerveSimulator::connect(SimulatedObject *device) {
                if (visualizer_) {
                    auto vis = std::make_shared<SwerveScreenVisualizer>(*this) ;
                    addVisualizer(vis) ;                    
                    visualizer_ = false ;
                }

                RobotSimBase::connect(device) ;
            }

            void SwerveSimulator::disconnect(SimulatedObject *device) {
                RobotSimBase::disconnect(device) ;
            }
        }
    }
}
