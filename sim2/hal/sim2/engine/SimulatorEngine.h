#pragma once

#include <engine/SimulationProperties.h>
#include <engine/EventsManager.h>
#include <engine/SimValue.h>
#include <engine/SimulatorMessages.h>
#include <vector>
#include <string>
#include <ostream>
#include <list>
#include <memory>
#include <thread>
#include <mutex>
#include <functional>

namespace xero
{
    namespace sim2
    {
        class SimulationEvent;
        class SimulationModel;
        class ModelFactoryBase;

        class SimulatorEngine
        {
        public:
            enum class ErrorCode
            {
                NoError,
                PropertyFileError,
                EventFileError,
                HALConfigFileError,
                NoSimulationThread,
                SimulatorNotRunning
            };

        private:
            SimulatorEngine();
            virtual ~SimulatorEngine();

        public:

            // Start the simulation thread in the background
            ErrorCode start();

            // End the simulation thread in the background
            ErrorCode end();

            // Parse the command line arguments
            bool parseCommandLineArgs(int ac, char **av);

            // Get the only instance of the simulation engine
            static SimulatorEngine &getEngine();

            // Create a new model instance
            std::shared_ptr<SimulationModel> createModelInstance(const std::string &model, const std::string &inst);

            // Register a new model factory
            void registerModelFactory(const std::string &name, ModelFactoryBase *factory) {
                model_factories_[name] = factory;
            }

            // Return the messages logger
            SimulatorMessages &getMessageOutput() {
                return msg_;
            }

            // Return simulation time in microseconds
            uint64_t getSimulationTime() const {
                return sim_time_;
            }

        private:
            void simulationThread();

            // Read the HAL configuration file
            bool readHalConfigFile(const std::string &path) ;
            bool getConfigFileCount(nlohmann::json obj, const char *name, int &cnt) ;

            // Start the simulation thread
            bool startThread();

            // Process any events that are due
            void runEvents();

            // Run any register HAL simulation functions
            void runHAL();

            // Run any models
            void runModels();

        private:
            static SimulatorEngine *theOne;

        private:
            // The output stream for simulator output
            SimulatorMessages msg_;

            // The simulator event file for the simulation
            std::string simfile_;

            // The simulator properties file
            std::string propfile_;

            // The HAL configuration file
            std::string halconfig_ ;

            // The simulator thread
            std::thread sim_thread_;

            // The mutex for arbitrating access to shared data
            std::mutex sim_mutex_;

            // WHile true, the simulator is running
            bool sim_running_;

            // The current simulation time
            uint64_t sim_time_;

            // The time step for one simulation cycle, should be much smaller than
            // the robot loop time step, 2 ms feels right
            uint64_t sim_time_step_;

            // The set of models for the simulator
            std::list<std::shared_ptr<SimulationModel>> models_;

            // The set of functions registered by HAL code
            std::list<std::function<void(SimulatorEngine &)>> hal_functions_;

            // The properties for the simulation
            std::shared_ptr<SimulationProperties> props_ ;

            // The events for the simulation
            std::shared_ptr<EventsManager> events_ ;

            // Model factories
            std::map<std::string, ModelFactoryBase *> model_factories_;
        };
    }
}