#include "RobotSimBase.h"
#include "SubsystemModel.h"
#include <cassert>
#include <chrono>

namespace xero {
    namespace sim {
        RobotSimBase *RobotSimBase::theOne = nullptr ;

        RobotSimBase &RobotSimBase::getRobotSimulator() {
            assert(theOne != nullptr) ;            
            return *theOne ;
        }

        RobotSimBase::RobotSimBase() {

        }

        RobotSimBase::~RobotSimBase() {            
        }

        void RobotSimBase::start() {
            current_time_ = 0.0 ;
            sim_time_step_ = 0.01 ;
            model_thread_ = std::thread(&RobotSimBase::simLoop, this) ;
            running_ = true ;
        }

        void RobotSimBase::stop() {
            running_ = false ;

            if (model_thread_.joinable())
                model_thread_.join() ;
        }

        double RobotSimBase::getTime() {
            std::lock_guard<std::mutex> lock(getLockMutex()) ;
            return current_time_ ;
        }

        double RobotSimBase::getTimeNoLock() {
            return current_time_ ;
        }

        void RobotSimBase::wait(double secs) {
            std::chrono::microseconds delay(100) ;            
            double now = getTime() ;

            //
            // The simulator thread moves time forward, pause until it
            // has moved forward the length of the desired delay
            //
            while (getTime() < now + secs)
                std::this_thread::sleep_for(delay) ;
        }

        void RobotSimBase::connect(SimulatedObject *device) {
        }

        void RobotSimBase::disconnect(SimulatedObject *device) {
        }

        void RobotSimBase::incrCurrentTime(double incr) {
            std::lock_guard<std::mutex> lock(getLockMutex()) ;
            current_time_ += incr ;            
        }

        void RobotSimBase::printOutput() {
            std::cout << "Time: " << current_time_  << std::endl; 
            for(auto model : models_) {
                std::cout << "    " << model->toString() << std::endl ;
            }
            std::cout << std::endl ;
        }

        //
        // This method is run from the simulator thread and not from
        // the robot thread.
        //
        void RobotSimBase::simLoop() {
            double last = getTime() ;
            std::chrono::microseconds delay(10) ;

            while (running_) {
                double now = getTime() ;
                for(auto model : models_)
                    model->run(now - last) ;

                last = now ;
                incrCurrentTime(sim_time_step_) ;
                std::this_thread::sleep_for(delay) ;

                if (printing_)
                    printOutput() ;
            }        
        }
    }
}
