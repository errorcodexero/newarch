#include <MessageLogger.h>
#include <Robot.h>
#include <motorencodersubsystem/MotorEncoderGoToAction.h>
#include <motorencodersubsystem/MotorEncoderHoldAction.h>
#include <motors/MotorController.h>
#include <actions/Action.h>
#include "Subsystem.h"
#include "CollectOffAction.h"
#include "Intake.h"

using namespace xero::base ;

namespace xero {
    namespace droid {
        std::string CollectOffAction::action_name("CollectOffAction");
        CollectOffAction::CollectOffAction(Intake &subsystem) : 
                    MotorEncoderGoToAction(subsystem, CollectOffAction::Target)
        {
        }
        
        void CollectOffAction::start() 
        {
            Intake &intake = dynamic_cast<Intake &>(getSubsystem()) ;            
            MotorEncoderGoToAction::start() ;
            intake.collector_->set(0.0) ;
        }

        void CollectOffAction::run() 
        {
            MotorEncoderGoToAction::run() ;
        }

        void CollectOffAction::cancel() {
            Intake &intake = dynamic_cast<Intake &>(getSubsystem()) ;             
            MotorEncoderGoToAction::cancel() ;
            intake.collector_.get()->set(0.0) ;                  //collector "off"
            setDone() ;
        }
    }
}
