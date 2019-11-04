#pragma once
#include "TubManipulatorModel.h"
#include <frc/RobotSimBase.h>
#include <LifterModel.h>
#include <TankDriveModel.h>
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

        namespace ranseur
        {
            class OIModel ;

            /// \brief A simulator for Phoenix.
            class ranseurSimulator : public xero::sim::RobotSimBase
            {
            public:
                /// \brief Create a simulator for Phoenix.
                /// \param paramfile the filename of the robot's parameter file
                ranseurSimulator(const std::string &paramfile);

                /// \brief Destroy the simulator.
                virtual ~ranseurSimulator();

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
                std::shared_ptr<xero::sim::ranseur::OIModel> oi_ ;
                std::shared_ptr<TubManipulatorModel> tub_manipulator_ ;
                bool visualizer_ ;
            };
        } // namespace ranseur
    } // namespace sim
} // namespace xero
