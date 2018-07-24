#pragma once
#include "DigitalInput.h"
#include "DriverStation.h"
#include "Encoder.h"
#include "Solenoid.h"
#include "TalonSRX.h"
#include "VictorSP.h"
#include "AHRS.h"
#include "Timer.h"
#include <list>
#include <memory>
#include <thread>
#include <chrono>
#include <mutex>
#include <string>
#include <map>

namespace xero {
    namespace sim {
        class SubsystemModel ;

        class RobotSimBase {
        public:
            static RobotSimBase &getRobotSimulator() ;

            RobotSimBase() ;
            virtual ~RobotSimBase() ;

            void setPrinting(bool p) {
                printing_ = p ;
            }

            bool setProperty(const std::string &prop)  ;

            virtual void start() ;
            virtual void stop() ;

            virtual double getTime() ;
            virtual void wait(double secs) ;

            virtual void connect(xero::sim::SimulatedObject *device) ;
            virtual void disconnect(xero::sim::SimulatedObject *device) ;

            std::mutex &getLockMutex() {
                return mutlock_ ;
            }

        protected:
            void addModel(std::shared_ptr<SubsystemModel> model) {
                models_.push_back(model) ;
            }

            std::list<std::shared_ptr<SubsystemModel>> &getModels() {
                return models_ ;
            }

        private:
            void simLoop() ;
            void incrCurrentTime(double incr) ;
            void printOutput() ;

        protected:
            static RobotSimBase *theOne ;

            std::list<std::shared_ptr<SubsystemModel>> models_ ;

            std::thread model_thread_ ;
            bool running_ ;
            std::mutex mutlock_ ;

            double current_time_ ;
            double sim_time_step_ ;

            bool printing_ ;

            std::ostream *output_ ;

            std::map<std::string, std::string> properties_ ;
        } ;
    }
}