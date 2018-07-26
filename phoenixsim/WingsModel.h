#pragma once

#include <SubsystemModel.h>

namespace xero  {
    namespace sim {
        namespace phoenix {
            class WingsModel : public SubsystemModel {
            public:
                WingsModel() ;
                virtual ~WingsModel() ;

                virtual std::string toString() ;
                virtual void run(double dt) ;
                virtual void inputChanged(SimulatedObject *obj) ;
                virtual void addSolenoid(frc::Solenoid *solenoid) ;

            private:
                bool wings_solenoid_state ;
                bool wings_state_ ;
                frc::Solenoid *wings_ ;
            } ;
        }
    }
}
