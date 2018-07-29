#include "RobotSimBase.h"
#include "SubsystemModel.h"
#include "PrintVisualizer.h"
#include <messageLogger.h>
#include <cassert>
#include <chrono>
#include <fstream>

using namespace xero::misc ;

namespace xero {
    namespace sim {
        RobotSimBase *RobotSimBase::theOne = nullptr ;

        RobotSimBase &RobotSimBase::getRobotSimulator() {
            assert(theOne != nullptr) ;            
            return *theOne ;

        }

        RobotSimBase::RobotSimBase(const std::string &paramfile) {
            assert(xero::sim::RobotSimBase::theOne == nullptr) ;
			xero::sim::RobotSimBase::theOne = this ;
            
            speed_ = 1.0 ;

            messageLogger logger ;
            parser_ = new SettingsParser(logger, 1) ;
            if (!parser_->readFile(paramfile)) {
                std::cerr << "Simulator cannot read parameter file '" << paramfile << "'" << std::endl ;
            }

            filestrm_ = nullptr ;
        }

        RobotSimBase::~RobotSimBase() {
            if (filestrm_ != nullptr)
                delete filestrm_ ;

            delete parser_ ;
        }

        void RobotSimBase::enablePrinting() {
            std::shared_ptr<PrintVisualizer> vis = std::make_shared<PrintVisualizer>(std::cerr) ;
            visualizers_.push_back(vis) ;
        }

        void RobotSimBase::enablePrinting(const std::string &name) {
            filestrm_ = new std::ofstream(name) ;
            if (filestrm_->bad() || filestrm_->fail()) {
                delete filestrm_ ;
                filestrm_ = nullptr ;
                return ;
            }

            std::shared_ptr<PrintVisualizer> vis = std::make_shared<PrintVisualizer>(*filestrm_) ;
            visualizers_.push_back(vis) ;
        }

        void RobotSimBase::start() {
            current_time_ = 0.0 ;
            sim_time_step_ = 0.01 ;
            model_thread_ = std::thread(&RobotSimBase::simLoop, this) ;
            running_ = true ;
        }

        void RobotSimBase::stop() {
            running_ = false ;
            waiting_ = true ;

            while (waiting_)
                std::this_thread::sleep_for(std::chrono::microseconds(10)) ;

            if (model_thread_.joinable())
                model_thread_.join() ;
        }

        double RobotSimBase::getTime() {
            std::lock_guard<std::mutex> lock(getLockMutex()) ;
            return current_time_ ;
        }

        void RobotSimBase::wait(double secs) {
            std::chrono::microseconds delay(100) ;            
            double now = current_time_ ;

            //
            // The simulator thread moves time forward, pause until it
            // has moved forward the length of the desired delay
            //
            while (current_time_ < now + secs)
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

        //
        // This method is run from the simulator thread and not from
        // the robot thread.
        //
        void RobotSimBase::simLoop() {
            double last = getTime() ;
            int64_t microloop = static_cast<int64_t>(sim_time_step_ * 1000000 * speed_) ;

            while (running_) {
                auto start = std::chrono::high_resolution_clock::now() ;
                double now = getTime() ;

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
