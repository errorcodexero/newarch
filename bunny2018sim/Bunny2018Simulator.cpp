#include "Bunny2018Simulator.h"
#include "TankDriveModel.h"
#include "Bunny2018ScreenVisualizer.h"
#include <cassert>
#include <iostream>
#include <cmath>

using namespace ctre::phoenix::motorcontrol::can ;
using namespace frc ;
using namespace xero::sim ;
using namespace xero::sim::bunny2018 ;

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

				bunnycollector_ = std::make_shared<CollectorModel>(*this) ;
				addModel(bunnycollector_) ;

				sorterintput_ = std::make_shared<SorterInputModel>(*this) ;
				addModel(sorterintput_) ;

				sorteroutput_ = std::make_shared<SorterOutputModel>(*this) ;
				addModel(sorteroutput_) ;

				hopper_ = std::make_shared<HopperModel>(*this);
				addModel(hopper_) ;

				sorter_ = std::make_shared<SorterModel>(*this) ;
				addModel(sorter_) ;

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
