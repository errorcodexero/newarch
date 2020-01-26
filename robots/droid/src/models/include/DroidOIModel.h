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
        class DroidOIModel : public xero::sim2::SimulationModel
        {
        public:
            DroidOIModel(xero::sim2::SimulatorEngine &engine, const std::string &inst);
            virtual ~DroidOIModel();

            virtual bool create() ;
            virtual void run(uint64_t microdt);
            virtual void processEvent(const std::string &name, const xero::sim2::SimValue &value) ;

        private:
            int index_ ;
            int automode_axis_ ;
            HAL_JoystickButtons buttons_ ;
            HAL_JoystickPOVs povs_ ;
            HAL_JoystickAxes axis_ ;
            HAL_JoystickDescriptor desc_ ;

            static std::array<double, 10> automode_axis_values_ ;
        };
    }
}