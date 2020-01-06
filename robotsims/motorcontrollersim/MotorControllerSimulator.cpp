#include "MotorControllerSimulator.h"
#include "MotorControllerScreenVisualizer.h"
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
        namespace motorcontroller
        {
            MotorControllerSimulator::MotorControllerSimulator(const std::string &paramfile) : RobotSimBase(paramfile)
            {
                tankdrive_ = std::make_shared<TankDriveModel>(*this, TankDriveModel::MotorType::MTSpark) ;
                addModel(tankdrive_) ;

                oi_ = std::make_shared<OIModel>(*this) ;
                addModel(oi_) ;      

                visualizer_ = false ;
            }

            MotorControllerSimulator::~MotorControllerSimulator() {
            }

            void MotorControllerSimulator::enableScreen() {
                visualizer_ = true ;
            }

            void MotorControllerSimulator::connect(SimulatedObject *device) {
                if (visualizer_) {
                    auto vis = std::make_shared<MotorControllerScreenVisualizer>(*this) ;
                    addVisualizer(vis) ;                    
                    visualizer_ = false ;
                }

                RobotSimBase::connect(device) ;
            }

            void MotorControllerSimulator::disconnect(SimulatedObject *device) {
                RobotSimBase::disconnect(device) ;
            }
        }
    }
}
