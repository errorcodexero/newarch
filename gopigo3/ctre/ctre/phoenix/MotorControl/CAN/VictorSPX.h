#pragma once


#include "../SensorCollection.h"
#include "../ControlMode.h"
#include "../NeutralMode.h"
#include "../IMotorController.h"
#include <cassert>

namespace ctre {
    namespace phoenix {
        namespace motorcontrol {
            namespace can {

                class VictorSPX : public IMotorController {
                public:
                    VictorSPX(int id) : IMotorController(id) {
                    }

                    virtual ~VictorSPX() {        
                    }

                    ctre::phoenix::motorcontrol::SensorCollection &GetSensorCollection() {
                        return sensors_ ;
                    }


                private:
                    ctre::phoenix::motorcontrol::SensorCollection sensors_ ;
                } ;
            }
        }
    }
}
