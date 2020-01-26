#include <IntakeModel.h>
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
        IntakeModel::IntakeModel(SimulatorEngine &engine, const std::string &inst) : SimulationModel(engine, "intake", inst)
        {
        }

        IntakeModel::~IntakeModel()
        {
        }

        void IntakeModel::processEvent(const std::string &name, const SimValue &value)
        {
        }

        bool IntakeModel::create()
        {
            intake_up_down_ = std::make_shared<SimulatedMotor>(*this, "updown") ;
            intake_spin_ = std::make_shared<SimulatedMotor>(*this, "spin") ;   

            return true ;
        }

        void IntakeModel::run(uint64_t microdt) 
        {
        }
    }
}
