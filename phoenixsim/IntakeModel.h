#pragma once

#include <SubsystemModel.h>

namespace xero  {
    namespace sim {
        namespace phoenix {
            class IntakeModel : public SubsystemModel {
            public:
                IntakeModel(RobotSimBase &simbase) ;
                virtual ~IntakeModel() ;

                virtual std::string toString() ;
                virtual void run(double dt) ;
                virtual void inputChanged(SimulatedObject *obj) ;
                virtual void addVictorSP(frc::VictorSP *victor) ;

                double getVoltage1() {
                    return voltage1_ ;
                }

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
