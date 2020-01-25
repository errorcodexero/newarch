#pragma once

#include <SimulatorEngine.h>
#include <SimulationModel.h>
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
        class SimulatorEngine ;

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

            virtual std::shared_ptr<SimulationModel> createModel(SimulatorEngine &engine, const std::string &inst) = 0;

            static std::list<std::string> &getModelList() {
                return models_;
            }

        protected:
            const std::string &getFactoryName() {
                return name_ ;
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

            virtual std::shared_ptr<SimulationModel> createModel(SimulatorEngine &engine, const std::string &inst) {
                auto ret = std::make_shared<T>(engine, inst);

                //
                // The registered factor name must match the model name for the time being.  This may change in the
                // future where a single factory can create models of a given type, but with different model names.  This
                // is not true now so enforce that the model name (as in the SimulationModel constructor call) matches
                // the factory name
                //
                assert(ret->getModelName() == getFactoryName()) ;

                return ret ;
            };        
        };
    }
}