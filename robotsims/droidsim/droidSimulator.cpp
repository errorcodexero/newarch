#include "droidSimulator.h"
#include "droidScreenVisualizer.h"
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
        namespace droid
        {
            droidSimulator::droidSimulator(const std::string &paramfile) : RobotSimBase(paramfile)
            {
                tankdrive_ = std::make_shared<TankDriveModel>(*this, TankDriveModel::MotorType::MTSpark) ;
                addModel(tankdrive_) ;

                oi_ = std::make_shared<OIModel>(*this) ;
                addModel(oi_) ;      

                visualizer_ = false ;
            }

            droidSimulator::~droidSimulator() {
            }

            void droidSimulator::enableScreen() {
                visualizer_ = true ;
            }

            void droidSimulator::connect(SimulatedObject *device) {
                if (visualizer_) {
                    auto vis = std::make_shared<droidScreenVisualizer>(*this) ;
                    addVisualizer(vis) ;                    
                    visualizer_ = false ;
                }

                RobotSimBase::connect(device) ;
            }

            void droidSimulator::disconnect(SimulatedObject *device) {
                RobotSimBase::disconnect(device) ;
            }
        }
    }
}
