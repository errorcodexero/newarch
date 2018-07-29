#pragma once

#include <SubsystemModel.h>

namespace xero  {
    namespace sim {
        namespace phoenix {
            class WingsModel : public SubsystemModel {
            public:
                WingsModel(RobotSimBase &simbase) ;
                virtual ~WingsModel() ;

                virtual std::string toString() ;
                virtual void run(double dt) ;
                virtual void inputChanged(SimulatedObject *obj) ;
                virtual void addSolenoid(frc::Solenoid *solenoid) ;

                bool getWings() {
                    return wings_solenoid_state_ ;
                }

            private:
                bool wings_solenoid_state_ ;
                int sol_channel_ ;
                frc::Solenoid *wings_ ;
            } ;
        }
    }
}
