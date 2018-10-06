#include "Bunny2018Simulator.h"
#include "TankDriveModel.h"
#include "Bunny2018ScreenVisualizer.h"
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
		namespace bunny2018
		{
			Bunny2018Simulator::Bunny2018Simulator(const std::string &paramfile) : RobotSimBase(paramfile)
			{
				tankdrive_ = std::make_shared<TankDriveModel>(*this) ;
				addModel(tankdrive_) ;

				oi_ = std::make_shared<OIModel>(*this) ;
				addModel(oi_) ;

				visualizer_ = false ;
			}

			Bunny2018Simulator::~Bunny2018Simulator() {
			}

			void Bunny2018Simulator::enableScreen() {
				visualizer_ = true ;
			}

			void Bunny2018Simulator::connect(SimulatedObject *device) {
				if (visualizer_) {
					auto vis = std::make_shared<Bunny2018ScreenVisualizer>(*this) ;
					addVisualizer(vis) ;					
					visualizer_ = false ;
				}

				RobotSimBase::connect(device) ;
			}

			void Bunny2018Simulator::disconnect(SimulatedObject *device) {
				RobotSimBase::disconnect(device) ;
			}
		}
	}
}
