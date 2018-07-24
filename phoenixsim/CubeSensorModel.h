#pragma once

#include <DigitalInput.h>
#include <SubsystemModel.h>

namespace xero  {
    namespace sim {
        namespace phoenix {
            class CubeSensorModel : public SubsystemModel {
            public:
                CubeSensorModel() ;
                virtual ~CubeSensorModel() ;

                virtual std::string toString() ;
                virtual void run(double dt) ;
                virtual void inputChanged(SimulatedObject *obj) ;
                virtual void addDigitalInput(frc::DigitalInput *input) ;

            private:       
                frc::DigitalInput *input_ ;
                bool cube_sensed_ ;
            } ;
        }
    }
}
