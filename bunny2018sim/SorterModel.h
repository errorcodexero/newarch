#pragma once

#include <SubsystemModel.h>

namespace xero  {
    namespace sim {
        namespace bunny2018 {
			/// \brief A model of Phoenix's intake.
            class SorterModel : public SubsystemModel {
            public:
				/// \brief Create a new model of the robot's intake.
				/// \param simbase a reference to the base for the robot simulator
                SorterModel(RobotSimBase &simbase) ;

				/// \brief Destroy the model.
                virtual ~SorterModel() ;

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
                virtual void addVictorSP(frc::VictorSP *victor) ;

				/// \brief Add a ball sorter inde sensor to the subsystem
				/// \param victor the digital IO to add
            	virtual void addDigitalInput(frc::DigitalInput *input) ;

				/// \brief Add an encoder to the model.
				/// \param encoder the encoder to add to the model
                virtual void addEncoder(frc::Encoder *encoder) ;				

				/// \brief Get the voltage applied to the first motor.
				/// \returns the voltage applied to the first motor
                double getVoltage() {
                    return voltage_ ;
                }

			private:
				int getEncoderValue() ;
               
            private:
                double voltage_ ;
                frc::VictorSP *motor_ ;
				frc::DigitalInput *index_ ;
				frc::Encoder *encoder_ ;
				double angle_ ;
				int ticks_per_revolution_ ;

                int motor_channel_ ;
				int encoder_channel_1_ ;
				int encoder_channel_2_ ;
				int index_sensor_ ;
            } ;
        }
    }
}