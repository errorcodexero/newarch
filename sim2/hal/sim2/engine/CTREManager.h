#pragma once

#include <engine/HardwareManager.h>
#include <map>

namespace xero
{
    namespace sim2
    {
        class CTREManager : public HardwareManager
        {
        public:
            CTREManager(SimulatorEngine &engine);
            virtual ~CTREManager();

            bool create(int index);
            bool destroy(int index);
            void set(int index double value);

        private:
            struct CTREStatus
            {
                double power_;
            };

        private:
            std::map<int, CTREStatus> status_;
        };
    }
}