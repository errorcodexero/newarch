#pragma once

#include <frc/SubsystemModel.h>

namespace xero  {
    namespace sim {
        /// \brief A model of Phoenix's lifter.
        class TurnTableModel : public SubsystemModel {
        public:
            /// \brief Create a new model of the robot's lifter.
            /// \param simbase a reference to the base for the robot simulator
            TurnTableModel(RobotSimBase &simbase) ;

            /// \brief Destroy the model.
            virtual ~TurnTableModel() ;

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
            virtual void addDevice(ctre::phoenix::motorcontrol::can::TalonSRX *motor);

            /// \brief Add an encoder to the model.
            /// \param encoder the encoder to add
            virtual void addDevice(frc::Encoder *encoder);

            /// \brief Add a digital input to the model.
            /// \param input the digital input to add
            virtual void addDevice(frc::DigitalInput *input) ;

            /// \brief Get the height of the lifter in inches.
            /// \returns the height of the lifter in inches
            double getAngle() {
                return angle_ ;
            }

            /// \brief Get the voltage applied to the lifter motor.
            /// \returns the voltage applied to the lifter motor
            double getPower() {
                return power_ ;
            }

            bool getMinLimit() const {
                return min_limit_ ;
            }

            bool getMaxLimit() const {
                return max_limit_ ;
            }

            virtual void generateDisplayInformation(std::list<std::string> &lines) ;

        private:
            frc::Encoder *enc_ ;
            frc::DigitalInput *min_limit_switch_ ;
            frc::DigitalInput *max_limit_switch_ ;            
            ctre::phoenix::motorcontrol::can::TalonSRX *motor1_ ;

            int encoder_1_ ;
            int encoder_2_ ;
            int min_limit_channel_ ;
            int max_limit_channel_ ;
            int motor_1_ ;

            double max_limit_boundary_ ;
            double min_limit_boundary_ ;
            double degrees_per_sec_per_volt_ ;
            double degrees_per_tick_ ;

            double angle_ ;
            double power_ ;
            int encoder_value_ ;
            
            bool min_limit_ ;
            bool max_limit_ ;
        } ;
    }
}
