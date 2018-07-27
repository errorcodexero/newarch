#include "SubsystemModel.h"
#include "RobotSimBase.h"

namespace xero {
    namespace sim {
        SubsystemModel::SubsystemModel() {            
        }

        SubsystemModel::~SubsystemModel() {
        }

        std::mutex &SubsystemModel::getLockMutex() {
            RobotSimBase &sim = RobotSimBase::getRobotSimulator() ;
            return sim.getLockMutex() ;            
        }
    }
}