#include "Conveyor.h"
#include "droidids.h"
#include "ConveyorAction.h"

#include <Robot.h>
#include <singlemotorsubsystem/SingleMotorPowerAction.h>
#include <SettingsParser.h>
#include <motors/MotorController.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <cassert>

using namespace xero::misc;
using namespace xero::base;

namespace xero {
    namespace droid {
        Conveyor::Conveyor(Subsystem *parent): Subsystem(parent, "conveyor") {
            sensor1_ = createSensor("hw:conveyor:sensor1") ;
            sensor2_ = createSensor("hw:conveyor:sensor2") ;
            sensor3_ = createSensor("hw:conveyor:sensor3") ;

            conveyor_belts_ = parent->getRobot().getMotorFactory()->createMotor("hw:conveyor:belts") ;
            turret_conveyor_ = parent->getRobot().getMotorFactory()->createMotor("hw:conveyor:turret") ;

            conveyor_belts_->setNeutralMode(xero::base::MotorController::NeutralMode::Brake) ;
            turret_conveyor_->setNeutralMode(xero::base::MotorController::NeutralMode::Brake) ;            
        }

        void Conveyor::computeState()
        {
            sensor1_state_ = !sensor1_->Get() ;
            sensor2_state_ = !sensor2_->Get() ;
            sensor3_state_ = !sensor3_->Get() ;

            frc::SmartDashboard::PutBoolean("s1", sensor1_state_) ;
            frc::SmartDashboard::PutBoolean("s2", sensor2_state_) ;
            frc::SmartDashboard::PutBoolean("s3", sensor3_state_) ;                        
        }

        void Conveyor::postHWInit() {
        }

        Conveyor::SensorPtr Conveyor::createSensor(std::string configName) 
        {
            int index = getRobot().getSettingsParser().getInteger(configName) ;

            std::cout << "Input " << index << std::endl ;
            return std::make_shared<frc::DigitalInput>(index) ;

        }

        void Conveyor::setMotor(double v) 
        {
            conveyor_belts_->set(v) ;
            turret_conveyor_->set(v) ;
        }

        bool Conveyor::canAcceptAction(ActionPtr ptr)
        {
            return true ;
        }
    }
}
