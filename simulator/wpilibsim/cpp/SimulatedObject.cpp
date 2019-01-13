#include <frc/SimulatedObject.h>
#include <frc/SubsystemModel.h>
#include <frc/RobotSimBase.h>

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