#pragma once

#include <SubsystemModel.h>

namespace xero  {
    namespace sim {
        namespace phoenix {
            /// \brief A model of Phoenix's wings.
            class WingsModel : public SubsystemModel {
            public:
                /// \brief Create a new model of the robot's wings.
                /// \param simbase a reference to the base for the robot simulator
                WingsModel(RobotSimBase &simbase) ;

                /// \brief Destroy the model.
                virtual ~WingsModel() ;

                /// \brief Convert the model to a string.
                /// \returns details about the wings' state
                virtual std::string toString() ;

                /// \brief Run the model.
                /// \param dt the time difference since the last code loop
                virtual void run(double dt) ;

                /// \brief one time initialization of the model
                virtual void init() {                   
                }

                /// \brief Update the state of the model.  Called when one of its inputs changes.
                /// \param obj the input object which changed state
                virtual void inputChanged(SimulatedObject *obj) ;

                /// \brief Add a solenoid to the model.
                /// \param solenoid the solenoid to add
                virtual void addSolenoid(frc::Solenoid *solenoid) ;

                /// \brief Get the state of the wings' locking piston.
                /// \returns the state of the wings' locking piston
                bool getWings() {
                    return wings_solenoid_state ;
                }

            private:
                bool wings_solenoid_state ;
                bool wings_state_ ;
                int sol_channel_ ;
                frc::Solenoid *wings_ ;
            } ;
        }
    }
}
