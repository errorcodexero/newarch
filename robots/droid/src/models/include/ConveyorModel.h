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
        class ConveyorModel : public xero::sim2::SimulationModel
        {
        public:
            ConveyorModel(xero::sim2::SimulatorEngine &engine, const std::string &inst);
            virtual ~ConveyorModel();

            virtual bool create() ;
            virtual void run(uint64_t microdt);
            virtual void processEvent(const std::string &name, const xero::sim2::SimValue &value) ;

        private:
            std::shared_ptr<xero::sim2::SimulatedMotor> motor_ ;
            int number_of_balls_ ;
            int sensor1_ ;
            int sensor2_ ;
            int sensor3_ ;
        };
    }
}