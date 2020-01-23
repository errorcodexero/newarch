#pragma once

#include <HardwareManager.h>
#include <map>
#include <mutex>

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
            bool set(int index, double value);
            bool get(int index, double &value);

            bool getEncoder(int index, double &value) ;
            bool setEncoder(int index, double value) ;

        private:
            struct CTREStatus
            {
                double power_;
                int32_t encoder_ ;
            };

        private:
            std::map<int, CTREStatus> status_;
            std::mutex lock_;
        };
    }
}