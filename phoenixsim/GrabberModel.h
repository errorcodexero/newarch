#pragma once

#include <SubsystemModel.h>

namespace xero  {
    namespace sim {
        namespace phoenix {
			/// \brief A model of Phoenix's grabber.
            class GrabberModel : public SubsystemModel {
            public:
				/// \brief Create a new model of the robot's grabber.
				/// \param simbase a reference to the base for robot simulator
                GrabberModel(RobotSimBase &simbase) ;

				/// \brief Destroy the model.
                virtual ~GrabberModel() ;

				/// \brief Convert the model to a string.
				/// \returns details about the grabber's state
                virtual std::string toString() ;

				/// \brief Run the model.
				/// \param dt the time difference since the last code loop
                virtual void run(double dt) ;

				/// \brief Update the state of the model.  Called when one of its inputs changes.
				/// \param obj the input object which changed state
                virtual void inputChanged(SimulatedObject *obj) ;

				/// \brief Add a Victor SP to the model.
				/// \param victor the Victor SP to add
                virtual void addVictorSP(frc::VictorSP *victor) ;

				/// \brief Add an encoder to the model.
				/// \param encoder the encoder to add to the model
                virtual void addEncoder(frc::Encoder *encoder) ;

				/// \brief Get the angle of the grabber arms.
				/// \returns the angle of the grabber arms
                double getAngle() {
                    return angle_ ;
                }

				// \brief Get the voltage applied to the grabber motors.
				/// \returns the voltage applied to the grabber motors
                double getVoltage() {
                    return voltage_ ;
                }

            private:
                int motor_channel_ ;
                frc::VictorSP *motor_ ;
                int encoder_input_1_ ;
                int encoder_input_2_ ;
                frc::Encoder *enc_ ;
                
                double cube_angle_ ;
                double angle_ ;
                double voltage_ ;
                double degrees_per_second_ ;
                double degrees_per_tick_ ;
                int encoder_base_ ;
            } ;
        }
    }
}
