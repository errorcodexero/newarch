#pragma once

#include <SubsystemModel.h>

namespace xero  {
    namespace sim {
        namespace bunny2018 {
			/// \brief A model of Phoenix's intake.
            class SorterInputModel : public SubsystemModel {
            public:
				/// \brief Create a new model of the robot's intake.
				/// \param simbase a reference to the base for the robot simulator
                SorterInputModel(RobotSimBase &simbase) ;

				/// \brief Destroy the model.
                virtual ~SorterInputModel() ;

				/// \brief Convert the model to a string.
				/// \returns details about the intake's state
                virtual std::string toString() ;

				/// \brief Run the model.
				/// \param dt the time difference since the last code loop
                virtual void run(double dt) ;

				/// \brief one time initialization at the start of the simulation
				virtual void init() {					
				}

				/// \brief Update the state of the model.  Called when one of its inputs changes.
				/// \param obj the input object which changed state
                virtual void inputChanged(SimulatedObject *obj) ;

				/// \brief Add a Victor SP to the model.
				/// \param victor the Victor SP to add
                virtual void addTalonSRX(ctre::phoenix::motorcontrol::can::TalonSRX *victor) ;

				/// \brief Get the voltage applied to the first motor.
				/// \returns the voltage applied to the first motor
                double getVoltage() {
                    return voltage_ ;
                }
               
            private:
                double voltage_ ;
            	ctre::phoenix::motorcontrol::can::TalonSRX *motor_ ;
                int motor_channel_ ;
            } ;
        }
    }
}