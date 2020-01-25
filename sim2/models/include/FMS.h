#pragma once
#include <SimulatedMotor.h>
#include <SimulationModel.h>
#include <mockdata/SPIData.h>
#include <memory>
#include <mutex>

namespace xero
{
    namespace models
    {
        class FMS : public xero::sim2::SimulationModel
        {
        public:
            FMS(xero::sim2::SimulatorEngine &engine, const std::string &inst);
            virtual ~FMS();

            virtual bool create() ;
            virtual void run(uint64_t microdt);

        private:
            enum class FMSState
            {
                Start,          // Disabled for 1 second
                Auto,           // Automode for 15 seconds
                Between,        // Disabled for 2 seconds
                Teleop,         // Teleop for 135 seconds
                Done,           // Disabled
            } ;

        private:
            FMSState state_ ;
            uint64_t period_start_time_ ;

            uint64_t start_time_ ;
            uint64_t auto_time_ ;
            uint64_t between_time_ ;
            uint64_t teleop_time_ ;
        };
    }
}