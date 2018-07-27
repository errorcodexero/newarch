#include "VictorSP.h"
#include "RobotSimBase.h"

using namespace xero::sim ;

namespace frc {
    VictorSP::VictorSP(int which) {
        which_ = which ;

        RobotSimBase &sim = RobotSimBase::getRobotSimulator() ;
        sim.connect(this) ;
    }

    VictorSP::~VictorSP() {
        RobotSimBase &sim = RobotSimBase::getRobotSimulator() ;
        sim.disconnect(this) ;        
    }

    void VictorSP::Set(double v) {
        value_ = v ;
        changed() ;
    }
}