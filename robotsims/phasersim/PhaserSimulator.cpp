#include "PhaserSimulator.h"
#include "PhaserScreenVisualizer.h"
#include "OIModel.h"
#include "LifterModel.h"
#include "TurnTableModel.h"
#include <frc/TankDriveModel.h>
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
        namespace phaser
        {
            PhaserSimulator::PhaserSimulator(const std::string &paramfile) : RobotSimBase(paramfile)
            {
                tankdrive_ = std::make_shared<TankDriveModel>(*this) ;
                addModel(tankdrive_) ;

                oi_ = std::make_shared<OIModel>(*this) ;
                addModel(oi_) ;        

                table_ = std::make_shared<TurnTableModel>(*this) ;
                addModel(table_) ;        

                visualizer_ = false ;
            }

            PhaserSimulator::~PhaserSimulator() {
            }

            void PhaserSimulator::enableScreen() {
                visualizer_ = true ;
            }

            void PhaserSimulator::connect(SimulatedObject *device) {
                if (visualizer_) {
                    auto vis = std::make_shared<PhaserScreenVisualizer>(*this) ;
                    addVisualizer(vis) ;                    
                    visualizer_ = false ;
                }

                RobotSimBase::connect(device) ;
            }

            void PhaserSimulator::disconnect(SimulatedObject *device) {
                RobotSimBase::disconnect(device) ;
            }
        }
    }
}
