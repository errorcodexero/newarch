#pragma once

#include "RobotSimBase.h"
#include <cstdint>
#include <thread>
#include <mutex>
#include <fstream>
#include <string>

namespace xero
{
	namespace sim
	{
		class TankDriveModel ;

		namespace phoenix
		{
			class LifterModel ;
			class WingsModel ;
			class IntakeModel ;
			class GrabberModel ;
			class CubeSensorModel ;

			/// \brief A simulator for Phoenix.
			class PhoenixSimulator : public xero::sim::RobotSimBase
			{
			public:
				/// \brief Create a simulator for Phoenix.
				/// \param paramfile the filename of the robot's parameter file
				PhoenixSimulator(const std::string &paramfile);

				/// \brief Destroy the simulator.
				virtual ~PhoenixSimulator();

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
				std::shared_ptr<xero::sim::TankDriveModel> TankDrive_ ;
				std::shared_ptr<xero::sim::phoenix::LifterModel> lifter_ ;
				std::shared_ptr<xero::sim::phoenix::WingsModel> wings_ ;
				std::shared_ptr<xero::sim::phoenix::GrabberModel> grabber_ ;
				std::shared_ptr<xero::sim::phoenix::CubeSensorModel> ballsensor_ ;
				std::shared_ptr<xero::sim::phoenix::IntakeModel> intake_ ;

				bool visualizer_ ;
			};
		} // namespace phoenix
	} // namespace sim
} // namespace xero
