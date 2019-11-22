#include <frc/RobotSimBase.h>
#include <frc/SubsystemModel.h>
#include <frc/PrintVisualizer.h>
#include "JoystickManager.h"
#include <MessageLogger.h>
#include <Robot.h>
#include <basegroups.h>
#include <cassert>
#include <chrono>
#include <fstream>

using namespace xero::misc ;
using namespace frc ;
using namespace ctre::phoenix::motorcontrol::can ;
using namespace rev ;

namespace xero {
    namespace sim {
        RobotSimBase *RobotSimBase::theOne = nullptr ;

        RobotSimBase &RobotSimBase::getRobotSimulator() {
            assert(theOne != nullptr) ;            
            return *theOne ;
        }

        RobotSimBase::RobotSimBase(const std::string &paramfile) {
            joysticks_ = nullptr ;
            assert(xero::sim::RobotSimBase::theOne == nullptr) ;
            xero::sim::RobotSimBase::theOne = this ;
            
            speed_ = 1.0 ;

            MessageLogger logger ;
            parser_ = new SettingsParser(logger, 1) ;
            if (!parser_->readFile(paramfile)) {
                std::cerr << "Simulator cannot read parameter file '" << paramfile << "'" << std::endl ;
                assert(0);
            }

            filestrm_ = nullptr ;
        }

        RobotSimBase::~RobotSimBase() {
            if (filestrm_ != nullptr)
                delete filestrm_ ;

            delete parser_ ;
            if (joysticks_ != nullptr)
                delete joysticks_ ;
        }

        std::shared_ptr<SubsystemModel> RobotSimBase::getModelByName(const std::string &name) {
            auto it = std::find_if(models_.begin(), models_.end(), [name](std::shared_ptr<SubsystemModel> model) { return model->getName() == name ; }) ;
            if (it == models_.end())
                return nullptr ;

            return *it ;
        }

        void RobotSimBase::enablePrinting() {
            std::shared_ptr<PrintVisualizer> vis = std::make_shared<PrintVisualizer>(*this, std::cerr) ;
            visualizers_.push_back(vis) ;
        }

        void RobotSimBase::enablePrinting(const std::string &name) {
            filestrm_ = new std::ofstream(name) ;
            if (filestrm_->bad() || filestrm_->fail()) {
                delete filestrm_ ;
                filestrm_ = nullptr ;
                return ;
            }

            std::shared_ptr<PrintVisualizer> vis = std::make_shared<PrintVisualizer>(*this, *filestrm_) ;
            visualizers_.push_back(vis) ;
        }

        void RobotSimBase::start(frc::SampleRobot *robot) {
            robot_ = robot ;
            current_time_ = 0.0 ;
            sim_time_step_ = 0.001 ;
            running_ = true ;            
            model_thread_ = std::thread(&RobotSimBase::simLoop, this) ;
        }

        void RobotSimBase::stop() {
            running_ = false ;
            waiting_ = true ;

            while (waiting_)
                std::this_thread::sleep_for(std::chrono::microseconds(10)) ;

            if (model_thread_.joinable())
                model_thread_.join() ;

            robot_ = nullptr ;
        }

        double RobotSimBase::getTime() {
            std::lock_guard<std::mutex> lock(getLockMutex()) ;
            return current_time_ ;
        }

        void RobotSimBase::wait(double secs) {
            std::chrono::microseconds delay(1) ;            
            double now = current_time_ ;

            //
            // The simulator thread moves time forward, pause until it
            // has moved forward the length of the desired delay
            //
            while (current_time_ < now + secs)
                std::this_thread::sleep_for(delay) ;
        }

        void RobotSimBase::connect(SimulatedObject *device) {
                
            TalonSRX *talon = dynamic_cast<TalonSRX *>(device) ;
            if (talon != nullptr) {
                for(auto model : getModels())
                    model->addDevice(talon) ;
            }

            CANSparkMax *spark = dynamic_cast<CANSparkMax*>(device) ;
            if (spark != nullptr)
            {
                for(auto model : getModels())
                    model->addDevice(spark) ;
            }

            VictorSPX *vicspx = dynamic_cast<VictorSPX *>(device) ;
            if (vicspx != nullptr) {
                for(auto model : getModels())
                    model->addDevice(vicspx) ;
            }   

            AnalogInput *ainput = dynamic_cast<AnalogInput *>(device) ;
            if (ainput != nullptr) {
                for(auto model : getModels())
                    model->addDevice(ainput) ;                 
            }

            Counter *cntr = dynamic_cast<Counter *>(device) ;
            if (cntr != nullptr) {
                for(auto model : getModels())
                    model->addDevice(cntr) ;                 
            }                   

            DigitalInput *dinput = dynamic_cast<DigitalInput *>(device) ;
            if (dinput != nullptr) {
                for(auto model : getModels())
                    model->addDevice(dinput) ;                 
            }               

            Encoder *encoder = dynamic_cast<Encoder *>(device) ;
            if (encoder != nullptr) {
                for(auto model : getModels())
                    model->addDevice(encoder) ;                    
            }

            VictorSP *victor = dynamic_cast<VictorSP *>(device) ;
            if (victor != nullptr) {
                for(auto model : getModels())
                    model->addDevice(victor) ;
            }

            Solenoid * solenoid = dynamic_cast<Solenoid *>(device) ;
            if (solenoid != nullptr) {
                for(auto model : getModels())
                    model->addDevice(solenoid) ;
            } 

            Relay * relay = dynamic_cast<Relay *>(device) ;
            if (relay != nullptr) {
                for(auto model : getModels())
                    model->addDevice(relay) ;
            }                            

            AHRS *navx = dynamic_cast<AHRS *>(device) ;
            if (navx != nullptr) {
                for(auto model : getModels())
                    model->addDevice(navx) ;                  
            }   

            DriverStation *ds = dynamic_cast<DriverStation *>(device) ;
            if (ds != nullptr) {
                for(auto model: getModels())
                    model->addDevice(ds) ;
            }
        }

        void RobotSimBase::disconnect(SimulatedObject *device) {
        }

        void RobotSimBase::incrCurrentTime(double incr) {
            std::lock_guard<std::mutex> lock(getLockMutex()) ;
            current_time_ += incr ;            
        }


        bool RobotSimBase::setProperty(const std::string &str) {
            size_t pos = str.find('=') ;
            if (pos == std::string::npos)
                return false ;

            std::string name = str.substr(0, pos) ;
            std::string value = str.substr(pos + 1) ;
            properties_[name] = value ;

            return true ;
        }


        bool RobotSimBase::hasProperty(const std::string &name) {
            auto it = properties_.find(name) ;
            return it != properties_.end() ;
        }

        const std::string &RobotSimBase::getProperty(const std::string &name) {
            auto it = properties_.find(name) ;
            return it->second ;
        }        

        void RobotSimBase::readJoysticks() {
            frc::DriverStation &ds = frc::DriverStation::GetInstance() ;

            joysticks_->poll() ;

            for(size_t i = 0 ; i < joysticks_->getCount() ; i++) {
                if (i >= ds.getStickCount())
                    continue ;

                auto &stick = ds.getStick(i) ;

                for(size_t j = 0 ; j < joysticks_->getButtonCount(i) ; j++) {
                    bool v = joysticks_->getButtonValue(i, j) ;
                    stick.setButtonValue(j + 1, v) ;
                }

                for(size_t j = 0 ; j < joysticks_->getPOVCount(i) ; j++) {
                    int v = joysticks_->getPOVValue(i, j) ;
                    stick.setPOVValue(j, v) ;
                }

                for(size_t j = 0 ; j < joysticks_->getAxisCount(i) ; j++) {
                    double v = joysticks_->getAxisValue(i, j) ;
                    stick.setAxisValue(j, v) ;
                }
            }
        }

        void RobotSimBase::dispatchEvent(const SimEvent &event) {
            int count = 0 ;
            int processed = 0 ;
            for(auto model : models_) {
                if (model->getName() == event.getModel()) {
                    if (model->processEvent(event.getName(), event.getValue()))
                        processed++ ;

                    count++ ;
                }
            }

            MessageLogger &logger = getRobotMessageLogger() ;

            if (count == 0) {
                logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_SIMULATOR) ;
                logger << "Event Dispatch Error: " << event.getModel() ;
                logger << " " << event.getName() ;
                logger << " " << event.getValue() ;
                logger << " - no model consumed the event" ;
                logger.endMessage() ;
            }
            else if (processed == 0) {
                logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_SIMULATOR) ;
                logger << "Event Dispatch Error: " << event.getModel() ;
                logger << " " << event.getName() ;
                logger << " " << event.getValue() ;
                logger << " - the model did not process the event" ;
                logger.endMessage() ;                
            }
            else if (processed > 1) {
                logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_SIMULATOR) ;
                logger << "Event Dispatch Error: " << event.getModel() ;
                logger << " " << event.getName() ;
                logger << " " << event.getValue() ;
                logger << " - more than one model consumed the event" ;
                logger.endMessage() ;                
            }            
        }

        MessageLogger &RobotSimBase::getRobotMessageLogger() {
            xero::base::Robot *xerorobot = dynamic_cast<xero::base::Robot *>(robot_) ;
            return xerorobot->getMessageLogger() ;
        }

        //
        // This method is run from the simulator thread and not from
        // the robot thread.
        //
        void RobotSimBase::simLoop() {
            double last = getTime() ;
            int64_t microloop = static_cast<int64_t>(sim_time_step_ * 1000000 * speed_) ;
            incrCurrentTime(sim_time_step_) ;

            for(auto model : models_)
                model->init() ;         

            while (running_) {
                auto start = std::chrono::high_resolution_clock::now() ;
                double now = getTime() ;

                while (events_.size() > 0 && events_.getFirstEventTime() < now) {
                    const SimEvent &event = events_.getFirstEvent() ;
                    MessageLogger &logger = getRobotMessageLogger() ;
                    logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_SIMULATOR) ;
                    logger << "Simulation Event: " << event.getModel() ;
                    logger << " " << event.getName() ;
                    logger << " " << event.getValue() ;
                    logger.endMessage() ;
                    
                    dispatchEvent(event) ;
                    events_.removeFirstEvent() ;
                }

                for(auto v:visualizers_)
                    v->beginCycle(now) ;

                for(auto model : models_)
                    model->run(now - last) ;

                for(auto v:visualizers_) {
                    for (auto model : models_)
                        v->visualizeSubsystem(model) ;
                }

                for(auto v:visualizers_)
                    v->endCycle() ;

                if (joysticks_ != nullptr && joysticks_->getCount() > 0)
                    readJoysticks() ;

                last = now ;
                incrCurrentTime(sim_time_step_) ;
 
                auto elapsed = std::chrono::high_resolution_clock::now() - start ;
                auto dur = std::chrono::duration_cast<std::chrono::microseconds>(elapsed) ;

                int64_t count = microloop - dur.count() ;
                if (count > 0)
                    std::this_thread::sleep_for(std::chrono::microseconds(count)) ;
            }

            waiting_ = false ;
        }
    }
}
