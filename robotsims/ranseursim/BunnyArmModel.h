#pragma once

#include <frc/SubsystemModel.h>
#include <frc/AnalogInput.h>
#include <frc/Solenoid.h>
#include <ctre/Phoenix.h>
#include <frc/Solenoid.h>

namespace xero  {
    namespace sim {
        namespace ranseur {
            class BunnyArmModel : public SubsystemModel
            {
            public:
                BunnyArmModel(RobotSimBase &sim) ;
                virtual ~BunnyArmModel() ;

                /// \brief Convert the model to a string.
                /// \returns details about the lifter's state
                virtual std::string toString() ;

                /// \brief Run the model.
                /// \param dt the time difference since the last code loop
                virtual void run(double dt);

                /// \brief Update the state of the model.  Called when one of its inputs changes.
                /// \param obj the input object which changed state
                virtual void inputChanged(SimulatedObject *obj);

                /// \brief Add a solenoid to the model.
                /// \param input the digital input to add
                virtual void addDevice(frc::Solenoid *input) ;

                /// \brief Generate display information for the model
                void generateDisplayInformation(std::list<std::string> &lines) ;

                bool isDeployed() {
                    return bunnyarm_state_ ;
                }

            private:
                int bunnyarm_solenoid_number_ ;
                bool bunnyarm_state_ ;
            } ;
        }
    }
}
