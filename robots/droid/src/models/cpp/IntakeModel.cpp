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
            intake_up_down_ = std::make_shared<SimulatedMotor>(*this, "hw:updown:motor") ;
            intake_spin_ = std::make_shared<SimulatedMotor>(*this, "hw:spin:motor") ;
            ticks_per_second_per_volt_ = getDouble("ticks_per_second_per_volt") ;
            ticks_ = 0 ;

            return true ;
        }

        void IntakeModel::run(uint64_t microdt) 
        {
            double tm = microdt / 1.0e6 ;
            double dist = intake_up_down_->get() * ticks_per_second_per_volt_ * tm ;

            ticks_ += dist ;
            int32_t value = static_cast<int32_t>(ticks_) ;
            intake_up_down_->setEncoder(value) ;
        }
    }
}
