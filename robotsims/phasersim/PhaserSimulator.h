#pragma once
#include "HatchHolderModel.h"
#include "CargoHolderModel.h"
#include "CargoIntakeModel.h"
#include "LineSensorModel.h"
#include "ClimberModel.h"
#include <frc/RobotSimBase.h>
#include <LifterModel.h>
#include <TankDriveModel.h>
#include <TurnTableModel.h>
#include <cstdint>
#include <thread>
#include <mutex>
#include <fstream>
#include <string>
#include <vector>

namespace xero
{
    namespace sim
    {
        class TankDriveModel ;

        namespace phaser
        {
            class OIModel ;

            /// \brief A simulator for Phoenix.
            class PhaserSimulator : public xero::sim::RobotSimBase
            {
            public:
                /// \brief Create a simulator for Phoenix.
                /// \param paramfile the filename of the robot's parameter file
                PhaserSimulator(const std::string &paramfile);

                /// \brief Destroy the simulator.
                virtual ~PhaserSimulator();

                /// \brief Connect a simulated hardware object to the simulator
                /// \param device the simulated hardware object to connect
                virtual void connect(xero::sim::SimulatedObject *device) ;

                /// \brief Disconnect a simulated hardware object from the simulator
                /// \param device the simulated hardware object to disconnect
                virtual void disconnect(xero::sim::SimulatedObject *device) ;

                /// \brief Enable the simulation visualizer.
                virtual void enableScreen() ;

            private:
                static constexpr double PI = 3.14159265359;
                std::shared_ptr<xero::sim::TankDriveModel> tankdrive_ ;
                std::shared_ptr<xero::sim::LifterModel> lifter_ ;
                std::shared_ptr<xero::sim::TurnTableModel> table_ ;
                std::shared_ptr<HatchHolderModel> hatch_holder_ ;
                std::shared_ptr<CargoHolderModel> cargo_holder_ ;                
                std::shared_ptr<CargoIntakeModel> cargo_intake_ ;   
                std::shared_ptr<ClimberModel> climber_ ;             
                std::shared_ptr<xero::sim::phaser::OIModel> oi_ ;     
                std::shared_ptr<LineSensorModel> front_ ;           
                std::shared_ptr<LineSensorModel> back_ ;   

                bool visualizer_ ;
            };
        } // namespace phoenix
    } // namespace sim
} // namespace xero
