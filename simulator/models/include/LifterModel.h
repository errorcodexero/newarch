#pragma once

#include <frc/SubsystemModel.h>

namespace xero  {
    namespace sim {
        /// \brief A model of Phoenix's lifter.
        class LifterModel : public SubsystemModel {
        public:
            /// \brief Create a new model of the robot's lifter.
            /// \param simbase a reference to the base for the robot simulator
            LifterModel(RobotSimBase &simbase) ;

            /// \brief Destroy the model.
            virtual ~LifterModel() ;

            /// \brief Convert the model to a string.
            /// \returns details about the lifter's state
            virtual std::string toString() ;

            /// \brief Run the model.
            /// \param dt the time difference since the last code loop
            virtual void run(double dt);

            /// \brief Update the state of the model.  Called when one of its inputs changes.
            /// \param obj the input object which changed state
            virtual void inputChanged(SimulatedObject *obj);

            /// \brief one time initialization at the start of a simulation
            virtual void init() {                   
            }

            /// \brief Add a Victor SP to the model.
            /// \param motor the Victor SP to add
            virtual void addTalonSRX(ctre::phoenix::motorcontrol::can::TalonSRX *motor);

            /// \brief Add an encoder to the model.
            /// \param encoder the encoder to add
            virtual void addEncoder(frc::Encoder *encoder);

            /// \brief Add a digital input to the model.
            /// \param input the digital input to add
            virtual void addDigitalInput(frc::DigitalInput *input) ;

            /// \brief Get the height of the lifter in inches.
            /// \returns the height of the lifter in inches
            double getHeight() {
                return height_ ;
            }

            /// \brief Get the voltage applied to the lifter motor.
            /// \returns the voltage applied to the lifter motor
            double getPower() {
                return power_ ;
            }

            /// \brief Get whether the lifter is at or beyond its top height limit.
            /// \returns true if the lifter is at or beyond its top height limit
            bool getTopLimit() {
                return height_ >= top_limit_height_ ;
            }

            /// \brief Get whether the lifter is at or beneath its bottom height limit.
            /// \returns true if the lifter is at or beneath its bottom height limit
            bool getBottomLimit() {
                return height_ <= bottom_limit_height_ ;
            }

            virtual void generateDisplayInformation(std::list<std::string> &lines) ;            

        private:
            frc::Encoder *enc_ ;
            frc::DigitalInput *bottom_limit_ ;
            frc::DigitalInput *top_limit_ ;
            ctre::phoenix::motorcontrol::can::TalonSRX *motor1_ ;

            int encoder_1_ ;
            int encoder_2_ ;
            int bottom_limit_channel_ ;
            int top_limit_channel_ ;
            int motor_1_ ;

            double bottom_limit_height_ ;
            double top_limit_height_ ;

            double inch_per_sec_per_volt_ ;
            double gravity_equivalent_speed_ ;

            double height_ ;
            double in_per_tick_ ;
            double power_ ;
            int encoder_base_ ;
        } ;
    }
}
