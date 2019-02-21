#pragma once

#include <frc/SubsystemModel.h>

namespace xero {
    namespace sim {
        namespace phaser {
            class HatchHolderModel : public SubsystemModel {
            public:
                HatchHolderModel(xero::sim::RobotSimBase &simbase) ;

                virtual ~HatchHolderModel() ;

                virtual void processEvent(const std::string &name, int value) ;

                /// \brief Convert the model to a string.
                /// \returns details about the lifter's state
                virtual std::string toString() ;

                /// \brief Run the model.
                /// \param dt the time difference since the last code loop
                virtual void run(double dt);

                /// \brief Update the state of the model.  Called when one of its inputs changes.
                /// \param obj the input object which changed state
                virtual void inputChanged(SimulatedObject *obj);

                /// \brief Add a digital input to the model.
                /// \param input the digital input to add
                virtual void addDevice(frc::DigitalInput *input) ;

                virtual void addDevice(frc::Solenoid *solenoid) ;

                bool hasHatch() const  {
                    return has_hatch_ ;
                }

                bool getExtensionState() const {
                    return extension_state_ ;
                }

                bool getFingerState() const {
                    return finger_state_ ;
                }

                virtual void generateDisplayInformation(std::list<std::string> &lines) ;

            private:
                frc::Solenoid *extension_solenoid_ ;
                frc::Solenoid *finger_solenoid_ ;
                frc::DigitalInput *hatch_sensor_ ;

                int extension_channel_ ;
                int finger_channel_ ;
                int hatch_sensor_channel_ ;

                bool extension_state_ ;
                bool finger_state_ ;
                bool has_hatch_ ;
            } ;
        }
    }
}