#pragma once

#include <SubsystemModel.h>

namespace xero  {
    namespace sim {
        namespace phoenix {
            class LifterModel : public SubsystemModel {
            public:
                LifterModel() ;
                virtual ~LifterModel() ;

                virtual void run(double dt);
	            virtual void inputChanged(SimulatedObject *obj);
                virtual std::string toString() ;

	            virtual void addVictorSP(frc::VictorSP *motor);
	            virtual void addEncoder(frc::Encoder *encoder);
                virtual void addDigitalInput(frc::DigitalInput *input) ;
                virtual void addSolenoid(frc::Solenoid *solenoid) ;

            private:
                frc::Encoder *enc_ ;
                frc::DigitalInput *bottom_limit_ ;
                frc::DigitalInput *top_limit_ ;
                frc::VictorSP *motor1_ ;
                frc::VictorSP *motor2_ ;
                frc::Solenoid *brake_ ;
                frc::Solenoid *gear_ ;

                double inch_per_sec_per_volt ;
                double height_ ;
                double bottom_height_ ;
                double in_per_tick_ ;
                double voltage_ ;
                bool brake_value_ ;
                bool highgear_ ;
                int encoder_base_ ;
            } ;
        }
    }
}