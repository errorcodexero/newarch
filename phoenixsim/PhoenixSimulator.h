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

			class PhoenixSimulator : public xero::sim::RobotSimBase
			{
			public:
				PhoenixSimulator(const std::string &paramfile);
				virtual ~PhoenixSimulator();

				virtual void connect(xero::sim::SimulatedObject *device) ;
				virtual void disconnect(xero::sim::SimulatedObject *device) ;

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
