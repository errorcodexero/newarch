#include "Conveyor.h"
#include "droidids.h"

#include <Robot.h>
#include <singlemotorsubsystem/SingleMotorPowerAction.h>
#include <SettingsParser.h>
#include <motors/MotorController.h>
#include <cassert>

using namespace xero::misc;
using namespace xero::base;

namespace xero {
    namespace droid {
        Conveyor::Conveyor(Subsystem *parent): SingleMotorSubsystem(parent, "conveyor", MSG_GROUP_CONVEYOR) {
            sensor1_ = createSensor("hw:conveyor:sensor1") ;
            sensor2_ = createSensor("hw:conveyor:sensor2") ;
            sensor3_ = createSensor("hw:conveyor:sensor3") ;

            conveyor_belts_ = parent->getRobot().getMotorFactory()->createMotor("hw:conveyor_belts") ;
            turret_conveyor_ = parent->getRobot().getMotorFactory()->createMotor("hw:turret_conveyor") ;
        }

        void Conveyor::computeState()
        {
            sensor1_state_ = sensor1_->Get() ;
            sensor2_state_ = sensor2_->Get() ;
            sensor3_state_ = sensor3_->Get() ;
        }

        void Conveyor::postHWInit() {
            setDefaultAction(std::make_shared<SingleMotorPowerAction>(*this, 0.0));
        }

        Conveyor::SensorPtr Conveyor::createSensor(std::string configName) 
        {
            int index = getRobot().getSettingsParser().getInteger(configName) ;
            return std::make_shared<frc::DigitalInput>(index) ;
        }

        void Conveyor::setMotor(double v) 
        {
            conveyor_belts_->set(v) ;
            turret_conveyor_->set(v) ;
        }
    }
}