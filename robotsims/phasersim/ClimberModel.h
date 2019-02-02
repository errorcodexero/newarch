#pragma once

#include <frc/SubsystemModel.h>

namespace xero {
    namespace sim {
        namespace phaser {
            class ClimberModel : public SubsystemModel {
            public:
                ClimberModel(xero::sim::RobotSimBase &simbase) ;

                virtual ~ClimberModel() ;

                /// \brief Convert the model to a string.
                /// \returns details about the lifter's state
                virtual std::string toString() ;

                /// \brief Run the model.
                /// \param dt the time difference since the last code loop
                virtual void run(double dt);

                /// \brief Update the state of the model.  Called when one of its inputs changes.
                /// \param obj the input object which changed state
                virtual void inputChanged(SimulatedObject *obj);

                virtual void addDevice(frc::Solenoid *solenoid) ;

                bool isDeployed() const {
                    return deploy_state_ ;
                }

                virtual void generateDisplayInformation(std::list<std::string> &lines) ;

            private:
                frc::Solenoid *deploy_solenoid_ ;

                int deploy_channel_ ;

                bool deploy_state_ ;
            } ;
        }
    }
}