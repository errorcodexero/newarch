#pragma once

#include <HardwareManager.h>
#include <map>
#include <mutex>

namespace xero
{
    namespace sim2
    {
        class REVManager : public HardwareManager
        {
        public:
            REVManager(SimulatorEngine &engine);
            virtual ~REVManager();

            bool create(int index);
            bool destroy(int index);
            bool set(int index, double value);
            bool get(int index, double &value);

        private:
            struct REVStatus
            {
                double power_;
            };

        private:
            std::map<int, REVStatus> status_;
            std::mutex lock_;
        };
    }
}