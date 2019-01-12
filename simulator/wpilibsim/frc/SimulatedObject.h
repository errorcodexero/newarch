#pragma once

#include <list>
#include <memory>
#include <mutex>

namespace xero {
    namespace sim {
        class SubsystemModel ;
        class RobotSimBase ;

        class SimulatedObject {
        public:
            SimulatedObject() ;
            virtual ~SimulatedObject() ;
            
            void addModel(SubsystemModel *ptr) {
                models_.push_back(ptr) ;
            }

            std::mutex &getLockMutex() ;

        protected:
            void changed() ;

        private:
            std::list<SubsystemModel *> models_ ;
        } ;
    }
}