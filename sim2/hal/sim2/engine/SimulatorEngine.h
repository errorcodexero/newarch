#pragma once

#include <engine/SimulationProperties.h>
#include <engine/EventsManager.h>
#include <engine/SimValue.h>
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

        class SimulatorEngine
        {
        public:
            enum class ErrorCode
            {
                NoError,
                PropertyFileError,
                EventFileError,
                NoSimulationThread,
                SimulatorNotRunning
            };

        public:
            SimulatorEngine();
            virtual ~SimulatorEngine();

            // Start the simulation thread in the background
            ErrorCode start();

            // End the simulation thread in the background
            ErrorCode end();

            // Parse the command line arguments
            bool parseCommandLineArgs(int ac, char **av);

            // Get the only instance of the simulation engine
            static SimulatorEngine &getEngine();

        private:
            void simulationThread();

            // Read the property file - called from the main thread
            bool readPropertyFile();

            // Read the event file - called from the main thread
            bool readEventFile();

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
            // The timing of the simulation
            double start_delay_ ;
            double auto_duration_ ;
            double teleop_duration_ ;
            double test_duration_ ;

            // The output stream for simulator output
            std::ostream *out_;

            // The simulator event file for the simulation
            std::string simfile_;

            // The simulator properties file
            std::string propfile_;

            // The simulator thread
            std::thread sim_thread_;

            // The mutex for arbitrating access to shared data
            std::mutex sim_mutex_;

            // WHile true, the simulator is running
            bool sim_running_;

            // The current simulation time
            double sim_time_;

            // The time step for one simulation cycle, should be much smaller than
            // the robot loop time step, 2 ms feels right
            double sim_time_step_;

            // The set of models for the simulator
            std::list<std::shared_ptr<SimulationModel>> models_;

            // The set of functions registered by HAL code
            std::list<std::function<void(SimulatorEngine &)>> hal_functions_;

            // The properties for the simulation
            SimulationProperties props_ ;

            // The events for the simulation
            EventsManager events_ ;
        };
    }
}