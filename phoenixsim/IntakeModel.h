#pragma once

#include <SubsystemModel.h>

namespace xero  {
    namespace sim {
        namespace phoenix {
            class IntakeModel : public SubsystemModel {
            public:
                IntakeModel() ;
                virtual ~IntakeModel() ;

                virtual std::string toString() ;
                virtual void run(double dt) ;
                virtual void inputChanged(SimulatedObject *obj) ;
                virtual void addVictorSP(frc::VictorSP *victor) ;

            private:
                double voltage1_ ;
                double voltage2_ ;
                frc::VictorSP *motor1_ ;
                frc::VictorSP *motor2_ ;
            } ;
        }
    }
}
