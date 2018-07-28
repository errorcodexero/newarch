#include "PhoenixSimulator.h"
#include "LifterModel.h"
#include "TankDriveModel.h"
#include "WingsModel.h"
#include "GrabberModel.h"
#include "CubeSensorModel.h"
#include "IntakeModel.h"
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
			PhoenixSimulator::PhoenixSimulator()
			{
				assert(xero::sim::RobotSimBase::theOne == nullptr) ;
				xero::sim::RobotSimBase::theOne = this ;

				tankdrive_ = std::make_shared<TankDriveModel>() ;
				addModel(tankdrive_) ;

				intake_ = std::make_shared<IntakeModel>() ;
				addModel(intake_) ;				

				lifter_ = std::make_shared<LifterModel>() ;
				addModel(lifter_) ;

				wings_ = std::make_shared<WingsModel>() ;
				addModel(wings_) ;

				grabber_ = std::make_shared<GrabberModel>() ;
				addModel(grabber_) ;

				ballsensor_ = std::make_shared<CubeSensorModel>() ;
				addModel(ballsensor_) ;
			}

			PhoenixSimulator::~PhoenixSimulator()
			{
			}

			void PhoenixSimulator::connect(SimulatedObject *device) {
				TalonSRX *talon = dynamic_cast<TalonSRX *>(device) ;
				if (talon != nullptr) {
					for(auto model : getModels())
						model->addTalon(talon) ;
				}

				DigitalInput *input = dynamic_cast<DigitalInput *>(device) ;
				if (input != nullptr) {
					for(auto model : getModels())
						model->addDigitalInput(input) ;					
				}				

				Encoder *encoder = dynamic_cast<Encoder *>(device) ;
				if (encoder != nullptr) {
					for(auto model : getModels())
						model->addEncoder(encoder) ;					
				}

				VictorSP *victor = dynamic_cast<VictorSP *>(device) ;
				if (victor != nullptr) {
					for(auto model : getModels())
						model->addVictorSP(victor) ;
				}

				Solenoid * solenoid = dynamic_cast<Solenoid *>(device) ;
				if (solenoid != nullptr) {
					for(auto model : getModels())
						model->addSolenoid(solenoid) ;
				}			

				AHRS *navx = dynamic_cast<AHRS *>(device) ;
				if (navx != nullptr) {
					for(auto model : getModels())
						model->addNavX(navx) ;					
				}	
			}

			void PhoenixSimulator::disconnect(SimulatedObject *device) {
				//
				// TODO
				//
			}
		}
	}
}
