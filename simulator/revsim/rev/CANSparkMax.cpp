#include "CANSparkMax.h"
#include <frc/RobotSimBase.h>

using namespace xero::sim ;

namespace rev
{
    CANSparkMax::CANSparkMax(int devid, MotorType type) : CANSparkMaxLowLevel(devid, type) {
        RobotSimBase &sim = RobotSimBase::getRobotSimulator();
        sim.connect(this);            
    }

    CANSparkMax::~CANSparkMax() {  
        RobotSimBase &sim = RobotSimBase::getRobotSimulator();
        sim.disconnect(this);                      
    }

}

