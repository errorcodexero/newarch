#pragma once

#include "SimulatorEngine.h"
#include "SimValue.h"
#include <string>
#include <map>

namespace xero
{
    namespace sim2
    {
        class SimulatorMessages ;
        class SimulatorEngine ;
        
        class SimulationModel
        {
        public:
            SimulationModel(SimulatorEngine &engine, const std::string &modelname, const std::string &instname);
            virtual ~SimulationModel();

            virtual bool create() = 0 ;

            const std::string &getModelName() const {
                return model_name_ ;
            }

            const std::string &getInstanceName() const {
                return inst_name_ ;
            }            

            virtual void run(uint64_t microdt) = 0;

            void setProperty(const std::string &name, const SimValue &value) {
                props_[name] = value;
            }

            bool hasProperty(const std::string &name) {
                auto it = props_.find(name) ;
                return it != props_.end() ;
            }

            SimValue getProperty(const std::string &name) {
                auto it = props_.find(name) ;
                return it->second ;
            }

            SimulatorMessages &getMessageOutput() {
                return engine_.getMessageOutput() ;
            }

            uint64_t getSimulationTime() {
                return engine_.getSimulationTime() ;
            }

            SimulatorEngine &getEngine() { return engine_ ;}

            int getInteger(const std::string &name) ;

        private:
            SimulatorEngine &engine_ ;
            std::map<std::string, SimValue> props_;
            std::string model_name_;
            std::string inst_name_ ;
        };
    }
}
