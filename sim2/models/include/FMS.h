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
            virtual void processEvent(const std::string &name, const xero::sim2::SimValue &value) ;

        private:
            double setProperty(const std::string &name, const xero::sim2::SimValue &value, double target) ;

        private:
            enum class FMSState
            {
                Start,          // Disabled for 1 second
                Auto,           // Automode for 15 seconds
                Between,        // Disabled for 2 seconds
                Teleop,         // Teleop for 135 seconds
                Closing,        // Disabled for 2 seconds
                Done,           // Done and disabled
            } ;

        private:
            FMSState state_ ;
            double period_start_time_ ;

            double start_time_ ;
            double auto_time_ ;
            double between_time_ ;
            double teleop_time_ ;
            double closing_time_ ;
        };
    }
}