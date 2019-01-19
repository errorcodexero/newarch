#include "PhaserSimulator.h"
#include "PhaserScreenVisualizer.h"
#include "OIModel.h"

#include <frc/TurnTableModel.h>
#include <frc/LifterModel.h>
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

                table_ = std::make_shared<TurnTableModel>(*this) ;
                addModel(table_) ;    

                lifter_ = std::make_shared<LifterModel>(*this) ;  
                addModel(lifter_) ;  

                hatch_holder_ = std::make_shared<HatchHolderModel>(*this) ;
                addModel(hatch_holder_) ;

                cargo_holder_ = std::make_shared<CargoHolderModel>(*this) ;
                addModel(cargo_holder_) ;                

                hatch_intake_ = std::make_shared<HatchIntakeModel>(*this) ;
                addModel(hatch_intake_) ;

                cargo_intake_ = std::make_shared<CargoIntakeModel>(*this) ;
                addModel(cargo_intake_) ;                

                climber_ = std::make_shared<ClimberModel>(*this) ;
                addModel(climber_) ;

                oi_ = std::make_shared<OIModel>(*this) ;
                addModel(oi_) ;        

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
