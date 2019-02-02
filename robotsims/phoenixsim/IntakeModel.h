#pragma once

#include <frc/SubsystemModel.h>

namespace xero  {
    namespace sim {
        namespace phoenix {
            /// \brief A model of Phoenix's intake.
            class IntakeModel : public SubsystemModel {
            public:
                /// \brief Create a new model of the robot's intake.
                /// \param simbase a reference to the base for the robot simulator
                IntakeModel(RobotSimBase &simbase) ;

                /// \brief Destroy the model.
                virtual ~IntakeModel() ;

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
                virtual void addDevice(frc::VictorSP *victor) ;

                /// \brief Get the voltage applied to the first motor.
                /// \returns the voltage applied to the first motor
                double getVoltage1() {
                    return voltage1_ ;
                }

                /// \brief Get the voltage applied to the second motor.
                /// \returns the voltage applied to the second motor
                double getVoltage2() {
                    return voltage2_ ;
                }
                
            private:
                double voltage1_ ;
                double voltage2_ ;
                frc::VictorSP *motor1_ ;
                frc::VictorSP *motor2_ ;
                int motor_channel_1_ ;
                int motor_channel_2_ ;
            } ;
        }
    }
}
