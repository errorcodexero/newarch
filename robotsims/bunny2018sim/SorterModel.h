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
                virtual void addTalonSRX(ctre::phoenix::motorcontrol::can::TalonSRX *talon) ;

                /// \brief Add a ball sorter inde sensor to the subsystem
                /// \param victor the digital IO to add
                virtual void addDigitalInput(frc::DigitalInput *input) ;

                /// \brief Add an encoder to the model.
                /// \param encoder the encoder to add to the model
                virtual void addEncoder(frc::Encoder *encoder) ;                

                /// \brief Get the voltage applied to the first motor.
                /// \returns the voltage applied to the first motor
                double getSorterVoltage() {
                    return sorter_voltage_ ;
                }

                double getInVoltage() {
                    return in_voltage_ ;
                }       

                double getOutVoltage() {
                    return out_voltage_ ;
                }                       

                double getAngle() {
                    return angle_ ;
                }

                bool getIndexValue() {
                    return index_value_ ;
                }

                bool getBallPresentState() {
                    return ball_present_state_ ;
                }

                bool getBallRedState() {
                    return ball_red_state_ ;
                }

            private:
                int getEncoderValue() ;
               
            private:
                double sorter_voltage_ ;
                double in_voltage_ ;
                double out_voltage_ ;

                ctre::phoenix::motorcontrol::can::TalonSRX *sortermotor_ ;
                ctre::phoenix::motorcontrol::can::TalonSRX *inmotor_ ;
                ctre::phoenix::motorcontrol::can::TalonSRX *outmotor_ ;                             
                frc::DigitalInput *index_ ;
                frc::DigitalInput *ball_present_ ;
                frc::DigitalInput *red_blue_ ;                              
                frc::Encoder *encoder_ ;

                double degrees_per_volt_per_second ;
                double angle_ ;
                int ticks_per_revolution_ ;

                int sorter_motor_channel_ ;
                int in_motor_channel_ ;
                int out_motor_channel_ ;                                
                int encoder_channel_1_ ;
                int encoder_channel_2_ ;
                int index_sensor_ ;
                int ball_present_index_ ;
                int red_build_index_ ;

                bool index_value_ ;

                bool ball_present_state_ ;
                bool ball_red_state_ ;
            } ;
        }
    }
}