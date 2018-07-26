#pragma once

#include <SubsystemModel.h>

namespace xero  {
    namespace sim {
        namespace phoenix {
            class GrabberModel : public SubsystemModel {
            public:
                GrabberModel() ;
                virtual ~GrabberModel() ;

                virtual std::string toString() ;
                virtual void run(double dt) ;
                virtual void inputChanged(SimulatedObject *obj) ;
                virtual void addVictorSP(frc::VictorSP *victor) ;
                virtual void addEncoder(frc::Encoder *encoder) ;

            private:
                frc::VictorSP *motor_ ;
                frc::Encoder *enc_ ;
                
                double cube_angle_ ;
                double angle_ ;
                double voltage_ ;
                double degrees_per_second_ ;
                double degrees_per_tick_ ;
                int encoder_base_ ;
            } ;
        }
    }
}
