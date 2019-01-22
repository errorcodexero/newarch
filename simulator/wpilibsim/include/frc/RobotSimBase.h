#pragma once
#include "DigitalInput.h"
#include "DriverStation.h"
#include "Encoder.h"
#include "Solenoid.h"
#include <ctre/phoenix/motorcontrol/can/TalonSRX.h>
#include "VictorSP.h"
#include "AHRS.h"
#include "Timer.h"
#include "JoystickManager.h"
#include <TankDriveModel.h>
#include <SettingsParser.h>
#include <list>
#include <memory>
#include <thread>
#include <chrono>
#include <mutex>
#include <string>
#include <map>
#include <fstream>

namespace frc {
    class SampleRobot ;
}

namespace xero {
    namespace sim {
        class SubsystemModel ;
        class Visualizer ;

        class RobotSimBase {
        public:
            static RobotSimBase &getRobotSimulator() ;

            RobotSimBase(const std::string &paramfile) ;
            virtual ~RobotSimBase() ;

            frc::SampleRobot *getRobot() {
                return robot_ ;
            }

            virtual void enablePrinting() ;
            virtual void enablePrinting(const std::string &name) ;
            virtual void enableScreen() {                
            }

            bool setProperty(const std::string &prop)  ;

            virtual void start(frc::SampleRobot *robot) ;
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

            bool hasProperty(const std::string &name) ;
            const std::string &getProperty(const std::string &name) ;

            std::shared_ptr<SubsystemModel> getModelByName(const std::string &name) ;

            double getRobotXPos() {
                if (tank_drive_model_ == nullptr)
                    return 0.0 ;

                return tank_drive_model_->getXPos() ;
            }

            double getRobotYPos() {
                if (tank_drive_model_ == nullptr)
                    return 0.0 ;

                return tank_drive_model_->getYPos() ;
            }           

        protected:
            void addModel(std::shared_ptr<SubsystemModel> model) {
                models_.push_back(model) ;

                auto tank = std::dynamic_pointer_cast<TankDriveModel>(model) ;
                if (tank != nullptr)
                    tank_drive_model_ = tank ;
            }

            std::list<std::shared_ptr<SubsystemModel>> &getModels() {
                return models_ ;
            }

            void addVisualizer(std::shared_ptr<Visualizer> vis) {
                visualizers_.push_back(vis) ;
            }

            void readJoysticks() ;

        private:
            void simLoop() ;
            void incrCurrentTime(double incr) ;

        private:
            static RobotSimBase *theOne ;

            std::list<std::shared_ptr<SubsystemModel>> models_ ;

            std::shared_ptr<TankDriveModel> tank_drive_model_ ;

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

            frc::SampleRobot *robot_ ;

            JoystickManager *joysticks_ ;
        } ;
    }
}
