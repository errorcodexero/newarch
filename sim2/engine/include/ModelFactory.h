#pragma once

#include <SimulatorEngine.h>
#include <string>
#include <memory>
#include <list>
#include <algorithm>

#define USE_SIMULATION_MODEL(cname, sname) xero::sim2::ModelFactory<cname> factory##cname(sname)

namespace xero
{
    namespace sim2
    {
        class SimulationModel ;

        class ModelFactoryBase
        {
        public:
            ModelFactoryBase(const std::string &name) {
                auto it = std::find(models_.begin(), models_.end(), name);

                // If this fires, there are two model factories trying to register the name name
                assert(it == models_.end());

                models_.push_back(name);
                name_ = name;

                SimulatorEngine &engine = SimulatorEngine::getEngine();
                engine.registerModelFactory(name, this);
            }

            virtual ~ModelFactoryBase() {                
            }

            virtual std::shared_ptr<SimulationModel> createModel() = 0;

            static std::list<std::string> &getModelList() {
                return models_;
            }

        private:
            std::string name_;

            static std::list<std::string> models_;
        };

        template <class T>
        class ModelFactory : public ModelFactoryBase
        {
        public:
            ModelFactory(const std::string &name) : ModelFactoryBase(name) {
            }

            virtual ~ModelFactory() {                
            }

            virtual std::shared_ptr<SimulationModel> createModel() {
                return std::make_shared<T>();
            };        
        };
    }
}