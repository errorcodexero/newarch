#include <frc/Solenoid.h>
#include <frc/RobotSimBase.h>

using namespace xero::sim ;

namespace frc {
    Solenoid::Solenoid(int channel) {
        channel_ = channel ;

        RobotSimBase &sim = RobotSimBase::getRobotSimulator() ;
        sim.connect(this) ;
    }

    Solenoid::~Solenoid() {
        RobotSimBase &sim = RobotSimBase::getRobotSimulator() ;
        sim.disconnect(this) ;        
    }

    void Solenoid::Set(bool v) {
        state_ = v ;
        changed() ;
    }
}