#pragma once
#include "DigitalInput.h"
#include "DriverStation.h"
#include "Encoder.h"
#include "Solenoid.h"
#include "TalonSRX.h"
#include "VictorSP.h"
#include "AHRS.h"
#include "Timer.h"
#include <SettingsParser.h>
#include <list>
#include <memory>
#include <thread>
#include <chrono>
#include <mutex>
#include <string>
#include <map>
#include <fstream>

namespace xero {
    namespace sim {
        class SubsystemModel ;
        class Visualizer ;

        class RobotSimBase {
        public:
            static RobotSimBase &getRobotSimulator() ;

            RobotSimBase(const std::string &paramfile) ;
            virtual ~RobotSimBase() ;

            virtual void enablePrinting() ;
            virtual void enablePrinting(const std::string &name) ;
            virtual void enableScreen() {                
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

            xero::misc::SettingsParser &getSettingsParser() {
                return *parser_ ;
            }

            void setSpeed(double s) {
                speed_ = s ;
            }

        protected:
            void addModel(std::shared_ptr<SubsystemModel> model) {
                models_.push_back(model) ;
            }

            std::list<std::shared_ptr<SubsystemModel>> &getModels() {
                return models_ ;
            }

            void addVisualizer(std::shared_ptr<Visualizer> vis) {
                visualizers_.push_back(vis) ;
            }

        private:
            void simLoop() ;
            void incrCurrentTime(double incr) ;

        protected:
            static RobotSimBase *theOne ;

            std::list<std::shared_ptr<SubsystemModel>> models_ ;

            std::thread model_thread_ ;
            bool running_ ;
            bool waiting_ ;
            std::mutex mutlock_ ;

            double current_time_ ;
            double sim_time_step_ ;

            bool printing_ ;

            std::map<std::string, std::string> properties_ ;

            xero::misc::SettingsParser *parser_ ;

            std::list<std::shared_ptr<Visualizer>> visualizers_ ;

            std::ofstream *filestrm_ ;

            double speed_ ;
        } ;
    }
}