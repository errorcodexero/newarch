#pragma once

#include <engine/SimulatorEngine.h>
#include <string>
#include <memory>
#include <list>
#include <algorithm>

namespace xero
{
    namespace sim2
    {
        class SimulationModel ;

        class ModelFactory
        {
        public:
            ModelFactory(const std::string &name) {
                auto it = std::find(models_.begin(), models_.end(), name);

                // If this fires, there are two model factories trying to register the name name
                assert(it == models_.end());

                models_.push_back(name);
                name_ = name;

                SimulatorEngine &engine = SimulatorEngine::getEngine();
                engine.registerModelFactory(name, this);
            }

            virtual ~ModelFactory() {                
            }

            virtual std::shared_ptr<SimulationModel> createModel() = 0;

        private:
            std::string name_;

            static std::list<std::string> models_;
        };
    }
}