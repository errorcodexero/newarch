#include "SubsystemModel.h"
#include "RobotSimBase.h"

namespace xero {
    namespace sim {
        SubsystemModel::SubsystemModel(RobotSimBase &simbase, const std::string &name) : simbase_(simbase) {
            name_ = name ;
        }

        SubsystemModel::~SubsystemModel() {
        }

        std::mutex &SubsystemModel::getLockMutex() {
            RobotSimBase &sim = RobotSimBase::getRobotSimulator() ;
            return sim.getLockMutex() ;            
        }
    }
}