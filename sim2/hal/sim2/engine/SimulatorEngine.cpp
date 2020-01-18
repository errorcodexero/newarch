#include <engine/SimulatorEngine.h>
#include <engine/SimulationEvent.h>
#include <engine/SimulationModel.h>
#include <engine/json.h>
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
            out_ = &std::cout;
        }

        SimulatorEngine::~SimulatorEngine()
        {
            assert(theOne == this);
            theOne = nullptr;
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
                        (*out_) << "error: command line option '--events' requires an additional argument" << std::endl;
                        return false;
                    }

                    simfile_ = *av++;
                    ac--;
                }
                else if (arg == "--props")
                {
                    if (ac == 0)
                    {
                        (*out_) << "error: command line option '--props' requires an additional argument" << std::endl;
                        return false;
                    }

                    propfile_ = *av++;
                    ac--;
                }
                else if (arg == "--halconfig")
                {
                    if (ac == 0)
                    {
                        (*out_) << "error: command line option '--halconfig' requires an additional argument" << std::endl;
                        return false;
                    }

                    halconfig_ = *av++;
                    ac--;                    
                }
            }

            return true;
        }

        SimulatorEngine::ErrorCode SimulatorEngine::start()
        {
            // Read the HAL configuration file
            if (!readHalConfigFile(halconfig_))
                return ErrorCode::HALConfigFileError ;

            // Read the properties file
            if (!props_.loadProperties(propfile_))
                return ErrorCode::PropertyFileError;

            // Read the event file
            if (!events_.loadEvents(simfile_))
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
                (*out_) << "Error: hal config file - cannot find section '" << name << "'" << std::endl ;                
                return false ;
            }

            try
            {
                cnt = devobj.value("count", -1) ;
            }
            catch(...)
            {
                (*out_) << "Error: hal config file - cannot get property 'count' in section '" << name << "'" << std::endl ;                  
                return false ;
            }

            if (cnt == -1)
            {
                (*out_) << "Warning: hal config file - property 'count' in section '" << name << "' does not exist, defaulting to 0" << std::endl ;                      
            }           

            return true ;
        }

        bool SimulatorEngine::readHalConfigFile(const std::string &path)
        {
            int cnt ;
            std::ifstream strm(path) ;

            if (!strm.is_open())
                return false ;

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

            return true ;
        }
    }
}
