#include <ConveyorModel.h>
#include <SimulatorEngine.h>
#include <SimulatedMotor.h>
#include <mockdata/DriverStationData.h>
#include <hal/HALBase.h>
#include <cassert>
#include <iostream>

using namespace xero::sim2;

namespace xero
{
    namespace models
    {
        ConveyorModel::ConveyorModel(SimulatorEngine &engine, const std::string &inst) : SimulationModel(engine, "intake", inst)
        {
        }

        ConveyorModel::~ConveyorModel()
        {
        }

        void ConveyorModel::processEvent(const std::string &name, const SimValue &value)
        {
        }

        bool ConveyorModel::create()
        {
            motor_ = std::make_shared<SimulatedMotor>(*this, "motor") ;

            sensor1_ = getInteger("sensor1") ;
            return true ;
        }

        void ConveyorModel::run(uint64_t microdt) 
        {
        }
    }
}
