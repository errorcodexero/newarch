#include <MessageLogger.h>
#include <Robot.h>
#include <motorencodersubsystem/MotorEncoderGoToAction.h>
#include <motorencodersubsystem/MotorEncoderHoldAction.h>
#include <motors/MotorController.h>
#include <actions/Action.h>
#include "Subsystem.h"
#include "CollectOnAction.h"
#include "Intake.h"

using namespace xero::base ;

namespace xero {
    namespace droid {
        std::string CollectOnAction::action_name("CollectOnAction");
        CollectOnAction::CollectOnAction(Intake &subsystem, bool spinner) : 
                MotorEncoderGoToAction(subsystem, CollectOnAction::Target)
        {
            collector_power_ = 0.0 ;

            if (spinner)
                collector_power_ = subsystem.getRobot().getSettingsParser().getDouble("intake:collector:motor:power") ;
        }     
        
        void CollectOnAction::start() {
            Intake &intake = dynamic_cast<Intake &>(getSubsystem()) ;
            MotorEncoderGoToAction::start() ;
            intake.collector_.get()->set(collector_power_) ;            
        }

        void CollectOnAction::run() {
            MotorEncoderGoToAction::run();
        }

        void CollectOnAction::cancel() {
            Intake &intake = dynamic_cast<Intake &>(getSubsystem()) ;            
            MotorEncoderGoToAction::cancel() ;
            intake.collector_.get()->set(0.0) ;              
            setDone() ;
        }

    }
}
