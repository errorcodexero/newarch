#include <frc/Relay.h>
#include <frc/RobotSimBase.h>

using namespace xero::sim ;

namespace frc {

    Relay::Relay(int channel, Relay::Direction dir)
    {
        channel_ = channel ;
        dir_ = dir ;

        RobotSimBase &sim = RobotSimBase::getRobotSimulator() ;
        sim.connect(this) ;        
    }

    Relay::~Relay() 
    {
    }

    void Relay::Set(Relay::Value v) {
        state_ = v ;
        changed() ;        
    }
}
