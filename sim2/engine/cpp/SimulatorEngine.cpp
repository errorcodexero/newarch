#include <SimulatorEngine.h>
#include <SimulationEvent.h>
#include <SimulationModel.h>
#include <ModelFactory.h>
#include <SimulatorStreamMessageSink.h>
#include <CTREManager.h>
#include <json.h>
#include <cassert>
#include <iostream>
#include <fstream>

namespace xero
{
    namespace sim2
    {
        SimulatorEngine *SimulatorEngine::theOne = nullptr;

        SimulatorEngine &SimulatorEngine::getEngine() {
            if (theOne == nullptr)
                theOne = new SimulatorEngine();
            return *theOne;
        }

        SimulatorEngine::SimulatorEngine()
        {
            auto sink = std::make_shared<SimulatorStreamMessageSink>(std::cout);
            msg_.addSink(sink);

            props_ = std::make_shared<SimulationProperties>(*this);
            events_ = std::make_shared<EventsManager>(*this);

            // Create the hardware managers
            ctre_mgr_ = std::make_shared<CTREManager>(*this);
        }

        SimulatorEngine::~SimulatorEngine()
        {
            assert(theOne == this);
            theOne = nullptr;
        }

        std::shared_ptr<SimulationModel> SimulatorEngine::createModelInstance(const std::string &model, const std::string &inst)
        {
            auto it = model_factories_.find(model);
            if (it == model_factories_.end())
                return nullptr;

            return it->second->createModel();
        }

        bool SimulatorEngine::parseCommandLineArgs(int ac, char **av)
        {
            ac--;
            av++;

            while (ac-- > 0)
            {
                std::string arg = *av++;

                if (arg == "--events")
                {
                    if (ac == 0)
                    {
                        msg_.startMessage(SimulatorMessages::MessageType::Error);
                        msg_ << "error: command line option '--events' requires an additional argument";
                        msg_.endMessage(sim_time_);
                        return false;
                    }

                    simfile_ = *av++;
                    ac--;
                }
                else if (arg == "--props")
                {
                    if (ac == 0)
                    {
                        msg_.startMessage(SimulatorMessages::MessageType::Error);
                        msg_ << "error: command line option '--props' requires an additional argument";
                        msg_.endMessage(sim_time_);
                        return false;
                    }

                    propfile_ = *av++;
                    ac--;
                }
                else if (arg == "--halconfig")
                {
                    if (ac == 0)
                    {
                        msg_.startMessage(SimulatorMessages::MessageType::Error);
                        msg_ << "error: command line option '--halconfig' requires an additional argument";
                        msg_.endMessage(sim_time_);
                        return false;
                    }

                    halconfig_ = *av++;
                    ac--;                    
                }
                else if (arg == "--debug")
                {
                    size_t index;

                    if (ac == 0)
                    {
                        msg_.startMessage(SimulatorMessages::MessageType::Error);
                        msg_ << "error: command line option '--debug' requires an additional argument";
                        msg_.endMessage(sim_time_);
                        return false;
                    }

                    std::string vstr = *av++;
                    ac--;
                    int v;
                    bool error = false;
                    try
                    {
                        v = std::stoi(vstr, &index);
                    }
                    catch(const std::exception& e)
                    {
                        error = true;
                    }

                    if (error || index != vstr.length())
                    {
                        msg_.startMessage(SimulatorMessages::MessageType::Error);
                        msg_ << "error: command line option '--debug' requires an additional integer argument, argument was not an integer";
                        msg_.endMessage(sim_time_);
                        return false;                        
                    }

                    msg_.setDebugLevel(v);
                }
                else
                {
                    msg_.startMessage(SimulatorMessages::MessageType::Error);
                    msg_ << "error: unknown command line option '" << arg << "'";
                    msg_.endMessage(sim_time_);
                    return false;                       
                }
                
            }

            return true;
        }

        SimulatorEngine::ErrorCode SimulatorEngine::start()
        {
            // Print information about models
            msg_.startMessage(SimulatorMessages::MessageType::Debug, 1);
            msg_ << "Models Defined: ";
            for(const std::string &model : ModelFactoryBase::getModelList())
                msg_ << " " << model;
            msg_.endMessage(sim_time_);

            // Read the HAL configuration file
            if (!readHalConfigFile(halconfig_))
                return ErrorCode::HALConfigFileError ;

            // Read the properties file
            if (!props_->loadProperties(propfile_))
                return ErrorCode::PropertyFileError;

            // Read the event file
            if (!events_->loadEvents(simfile_))
                return ErrorCode::EventFileError;

            if (!startThread())
                return ErrorCode::NoSimulationThread;

            return ErrorCode::NoError;
        }

        SimulatorEngine::ErrorCode SimulatorEngine::end()
        {
            if (!sim_running_)
                return ErrorCode::SimulatorNotRunning;

            sim_running_ = false;
            sim_thread_.join();

            return ErrorCode::NoError;
        }

        bool SimulatorEngine::startThread()
        {
            sim_running_ = true;
            sim_time_ = 0.0;
            sim_time_step_ = 0.002;
            sim_thread_ = std::thread(&SimulatorEngine::simulationThread, this);

            return true;
        }

        void SimulatorEngine::simulationThread()
        {
            while(sim_running_)
            {
                // Update the simulators running time
                sim_time_ += sim_time_step_;

                // Process events read into the events file
                runEvents();

                // Run HAL layer processes
                runHAL();

                // Run models
                runModels();

                // Yield to give the robot thread time to run
                std::this_thread::yield();
            }
        }

        void SimulatorEngine::runEvents()
        {
        }

        void SimulatorEngine::runModels()
        {
            for(auto model : models_)
                model->run(*this);
        }

        void SimulatorEngine::runHAL()
        {
            for(auto fun : hal_functions_)
                fun(*this);
        }

        bool SimulatorEngine::getConfigFileCount(nlohmann::json obj, const char *name, int &cnt)
        {
            nlohmann::json devobj, cntobj ;
            
            try
            {
                devobj = obj.at(name);
            }
            catch(...)
            {
                msg_.startMessage(SimulatorMessages::MessageType::Error);
                msg_ << "hal config file - cannot find section '" << name << "'";
                msg_.endMessage(sim_time_);
                return false ;
            }

            try
            {
                cnt = devobj.value("count", -1) ;
            }
            catch(...)
            {
                msg_.startMessage(SimulatorMessages::MessageType::Error);
                msg_ << "hal config file - cannot get property 'count' in section '" << name << "'";
                msg_.endMessage(sim_time_);
                return false ;
            }

            if (cnt == -1)
            {
                msg_.startMessage(SimulatorMessages::MessageType::Warning);
                msg_ << "hal config file - property 'count' in section '" << name << "' does not exist, defaulting to 0";
                msg_.endMessage(sim_time_);
            }           
           
            msg_.startMessage(SimulatorMessages::MessageType::Debug, 5);
            msg_ << "  " << cnt << " " << name << " defined";
            msg_.endMessage(sim_time_);   

            return true ;
        }

        bool SimulatorEngine::readHalConfigFile(const std::string &path)
        {
            int cnt ;
            std::ifstream strm(path) ;

            if (!strm.is_open())
            {                
                msg_.startMessage(SimulatorMessages::MessageType::Error);
                msg_ << "could not open hal config file '" << path << "'";
                msg_.endMessage(sim_time_);     

                return false ;
            }

            msg_.startMessage(SimulatorMessages::MessageType::Debug, 1);
            msg_ << "reading hal config file '" << path << "'";
            msg_.endMessage(sim_time_);                   

            nlohmann::json obj = nlohmann::json::parse(strm);
            strm.close();

            if (!getConfigFileCount(obj, "accumulators", cnt))
                return false ;
              

            if (!getConfigFileCount(obj, "analog_triggers", cnt))
                return false ;

            if (!getConfigFileCount(obj, "analog_inputs", cnt))
                return false ;

            if (!getConfigFileCount(obj, "analog_outputs", cnt))
                return false ;

            if (!getConfigFileCount(obj, "counters", cnt))
                return false ;

            if (!getConfigFileCount(obj, "digitalios", cnt))
                return false ;

            if (!getConfigFileCount(obj, "pwms", cnt))
                return false ;

            if (!getConfigFileCount(obj, "encoders", cnt))
                return false ;

            if (!getConfigFileCount(obj, "interrupts", cnt))
                return false ;

            if (!getConfigFileCount(obj, "relays", cnt))
                return false ;

            if (!getConfigFileCount(obj, "pcms", cnt))
                return false ;

            if (!getConfigFileCount(obj, "solenoids", cnt))
                return false ;

            if (!getConfigFileCount(obj, "pdps", cnt))
                return false ;

            if (!getConfigFileCount(obj, "pdpchannels", cnt))
                return false ;

            msg_.startMessage(SimulatorMessages::MessageType::Debug, 0);
            msg_ << "hal config file read sucessfully";
            msg_.endMessage(sim_time_);                

            return true ;
        }
    }
}
