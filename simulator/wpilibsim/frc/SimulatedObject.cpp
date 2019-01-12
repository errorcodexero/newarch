#include "SimulatedObject.h"
#include "SubsystemModel.h"
#include "RobotSimBase.h"

namespace xero {
    namespace sim {
        SimulatedObject::SimulatedObject() {
        }

        SimulatedObject::~SimulatedObject() {            
        }

        std::mutex &SimulatedObject::getLockMutex() {
            RobotSimBase &sim = RobotSimBase::getRobotSimulator() ;
            return sim.getLockMutex() ;
        }

        void SimulatedObject::changed() {
            for(auto ptr: models_)
                ptr->inputChanged(this) ;
        }
    }
}