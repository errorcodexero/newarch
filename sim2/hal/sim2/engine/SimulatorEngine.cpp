#include <engine/SimulatorEngine.h>
#include <engine/SimulationEvent.h>
#include <engine/SimulationModel.h>
#include <cassert>
#include <iostream>

namespace xero
{
    namespace sim2
    {

        SimulatorEngine *SimulatorEngine::theOne = nullptr;

        SimulatorEngine::SimulatorEngine()
        {
            assert(theOne == nullptr);
            theOne = this;

            out_ = &std::cout;
        }

        SimulatorEngine::~SimulatorEngine()
        {
            assert(theOne == this);
            theOne = nullptr;
                else if (arg == "--start")
                {
                    
                }        }

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
            }

            return true;
        }

        SimulatorEngine::ErrorCode SimulatorEngine::start()
        {
            // Read the properties file
            if (props_.loadProperties(propfile_))
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
    }
}
