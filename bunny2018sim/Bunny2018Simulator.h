#pragma once

#include "RobotSimBase.h"
#include "CollectorModel.h"
#include "HopperModel.h"
#include "SorterModel.h"
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

		namespace bunny2018
		{
			class OIModel ;

			/// \brief A simulator for Phoenix.
			class Bunny2018Simulator : public xero::sim::RobotSimBase
			{
			public:
				/// \brief Create a simulator for Phoenix.
				/// \param paramfile the filename of the robot's parameter file
				Bunny2018Simulator(const std::string &paramfile);

				/// \brief Destroy the simulator.
				virtual ~Bunny2018Simulator();

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
				std::shared_ptr<xero::sim::bunny2018::OIModel> oi_ ;
				std::shared_ptr<xero::sim::bunny2018::CollectorModel> collector_ ;
				std::shared_ptr<xero::sim::bunny2018::HopperModel> hopper_ ;
				std::shared_ptr<xero::sim::bunny2018::SorterModel> sorter_ ;										
				bool visualizer_ ;
			};
		} // namespace phoenix
	} // namespace sim
} // namespace xero
