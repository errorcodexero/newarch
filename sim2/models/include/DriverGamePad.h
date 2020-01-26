#pragma once
#include <SimulatedMotor.h>
#include <SimulationModel.h>
#include <mockdata/DriverStationData.h>
#include <memory>
#include <mutex>

namespace xero
{
    namespace models
    {
        class DriverGamePad : public xero::sim2::SimulationModel
        {
        public:
            DriverGamePad(xero::sim2::SimulatorEngine &engine, const std::string &inst);
            virtual ~DriverGamePad();

            virtual bool create() ;
            virtual void run(uint64_t microdt);
            virtual void processEvent(const std::string &name, const xero::sim2::SimValue &value) ;

        private:
            int index_ ;
            HAL_JoystickButtons buttons_ ;
            HAL_JoystickPOVs povs_ ;
            HAL_JoystickAxes axis_ ;
            HAL_JoystickDescriptor desc_ ;
        };
    }
}