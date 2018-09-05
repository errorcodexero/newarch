#include "PhoenixSimulator.h"
#include "LifterModel.h"
#include "TankDriveModel.h"
#include "WingsModel.h"
#include "GrabberModel.h"
#include "CubeSensorModel.h"
#include "IntakeModel.h"
#include "PhoenixScreenVisualizer.h"
#include <cassert>
#include <iostream>

using namespace ctre::phoenix::motorcontrol::can ;
using namespace frc ;
using namespace xero::sim ;

namespace xero
{
	namespace sim
	{
		namespace phoenix
		{
			PhoenixSimulator::PhoenixSimulator(const std::string &paramfile) : RobotSimBase(paramfile)
			{
				tankdrive_ = std::make_shared<TankDriveModel>(*this) ;
				addModel(tankdrive_) ;

				lifter_ = std::make_shared<LifterModel>(*this) ;
				addModel(lifter_) ;

				wings_ = std::make_shared<WingsModel>(*this) ;
				addModel(wings_) ;

				grabber_ = std::make_shared<GrabberModel>(*this) ;
				addModel(grabber_) ;

				ballsensor_ = std::make_shared<CubeSensorModel>(*this) ;
				addModel(ballsensor_) ;

				intake_ = std::make_shared<IntakeModel>(*this) ;
				addModel(intake_) ;

				oi_ = std::make_shared<OIModel>(*this) ;
				addModel(oi_) ;

				visualizer_ = false ;
			}

			PhoenixSimulator::~PhoenixSimulator() {
			}

			void PhoenixSimulator::enableScreen() {
				visualizer_ = true ;
			}

			void PhoenixSimulator::connect(SimulatedObject *device) {
				if (visualizer_) {
					auto vis = std::make_shared<PhoenixScreenVisualizer>(*this) ;
					addVisualizer(vis) ;					
					visualizer_ = false ;
				}

				RobotSimBase::connect(device) ;
			}

			void PhoenixSimulator::disconnect(SimulatedObject *device) {
				RobotSimBase::disconnect(device) ;
			}
		}
	}
}
