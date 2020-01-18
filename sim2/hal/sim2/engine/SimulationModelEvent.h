#pragma once

#include <engine/SimulationEvent.h>
#include <engine/SimValue.h>
#include <string>

namespace xero
{
    namespace sim2
    {
        class SimulationModelEvent : public SimulationEvent
        {
        public:

        public:
            SimulationModelEvent(double t, const std::string &model, const std::string &inst, const std::string &name, const SimValue &value) : SimulationEvent(t)
            {
                model_ = model;
                inst_ = inst;
                name_ = name;
                value_ = value;
            }

            virtual ~SimulationModelEvent()
            {
            }

            const std::string &modelName() const {
                return model_;
            }

            const std::string &instance() const {
                return inst_;
            }

            const std::string &name() const {
                return name_;
            }

            const SimValue &value() const {
                return value_;
            }

            virtual std::string toString() const {
                std::string ret;

                ret += "ModelEvent:";
                ret += "time = " + std::to_string(time());
                ret += ", model = " + model_;
                ret += ", instance = " + inst_;
                ret += ", name = " + name_;
                ret += ", value = " + value_.toString();

                return ret;
            }

        private:
            std::string model_;
            std::string inst_;
            std::string name_;
            SimValue value_;
        };
    }
}