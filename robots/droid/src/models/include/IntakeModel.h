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
        class IntakeModel : public xero::sim2::SimulationModel
        {
        public:
            IntakeModel(xero::sim2::SimulatorEngine &engine, const std::string &inst);
            virtual ~IntakeModel();

            virtual bool create() ;
            virtual void run(uint64_t microdt);
            virtual void processEvent(const std::string &name, const xero::sim2::SimValue &value) ;

        private:
            std::shared_ptr<xero::sim2::SimulatedMotor> intake_up_down_ ;
            std::shared_ptr<xero::sim2::SimulatedMotor> intake_spin_ ;
            double ticks_ ;
            double ticks_per_second_per_volt_ ;            
        };
    }
}