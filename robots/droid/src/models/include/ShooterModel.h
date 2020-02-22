#pragma once
#include <SimulatedMotor.h>
#include <SimulationModel.h>
#include <mockdata/DriverStationData.h>
#include <memory>
#include <mutex>
#include <array>

namespace xero
{
    namespace models
    {
        class ShooterModel : public xero::sim2::SimulationModel
        {
        public:
            ShooterModel(xero::sim2::SimulatorEngine &engine, const std::string &inst);
            virtual ~ShooterModel();

            virtual bool create() ;
            virtual void run(uint64_t microdt);
            virtual void processEvent(const std::string &name, const xero::sim2::SimValue &value) ;

        private:
            constexpr static const char *KVPropName = "kv" ;
            constexpr static const char *ChangePropName = "change" ;

        private:
            std::shared_ptr<xero::sim2::SimulatedMotor> shooter_ ;
            double current_speed_rpm_ ;
            double kv_ ;
            double rpm_change_per_second_ ;
            double encoder_count_ ;
        };
    }
}