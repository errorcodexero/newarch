#pragma once

#include <SimulationProperties.h>
#include <EventsManager.h>
#include <SimValue.h>
#include <SimulatorMessages.h>
#include <vector>
#include <string>
#include <ostream>
#include <list>
#include <memory>
#include <mutex>
#include <functional>

namespace xero
{
    namespace sim2
    {
        class SimulationEvent;
        class SimulationModel;
        class ModelFactoryBase;
        class CTREManager;
        class REVManager ;

        class SimulatorEngine
        {
        public:
            enum class ErrorCode
            {
                NoError,
                BadCommandLine,
                PropertyFileError,
                EventFileError,
                HALError
            };

        private:
            SimulatorEngine();
            virtual ~SimulatorEngine();

        public:

            // Initialize the simulation engine
            ErrorCode start(int ac, char **av);

            // Run the simulator to catch up with the robot time
            void runSim() ;

            // End the simulation engine
            ErrorCode end();

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

            SimulationProperties &getProps() {
                return *props_ ;
            }

            // Get the hardware managers
            std::shared_ptr<CTREManager> getCTREManager() { return ctre_mgr_; }
            std::shared_ptr<REVManager> getREVManager() { return rev_mgr_; }            

            // Access to the HAL mock data for models
            void setEncoder(int indexA, int indexB, int32_t value) ;
            double getPWM(int index) ;

        private:
            // Process any events that are due
            void runEvents();

            // Run any models
            void runModels();

            // Register for events from the HAL
            bool registerForHALEvents();
            
            // Parse the command line arguments
            bool parseCommandLineArgs(int ac, char **av);

            std::shared_ptr<SimulationModel> findModelInstance(const std::string &model, const std::string &inst);            

        private:
            static SimulatorEngine *theOne;

        private:
            // The output stream for simulator output
            SimulatorMessages msg_;

            // The simulator event file for the simulation
            std::string simfile_;

            // The simulator properties file
            std::string propfile_;

            // The current simulation time
            uint64_t sim_time_;

            // The time step for one simulation cycle, should be much smaller than
            // the robot loop time step, 2 ms feels right
            uint64_t sim_time_step_;

            // The set of models for the simulator
            std::list<std::shared_ptr<SimulationModel>> models_;

            // The properties for the simulation
            std::shared_ptr<SimulationProperties> props_ ;

            // The events for the simulation
            std::shared_ptr<EventsManager> events_ ;

            // Model factories
            std::map<std::string, ModelFactoryBase *> model_factories_;

            // Hardware Managers
            std::shared_ptr<CTREManager> ctre_mgr_;
            std::shared_ptr<REVManager> rev_mgr_;
        };
    }
}