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
            rev_power_ = -1.0 ;
        }
        
        void CollectOffAction::start() 
        {
            Intake &intake = dynamic_cast<Intake &>(getSubsystem()) ;            
            MotorEncoderGoToAction::start() ;

            if (!isDone())
                intake.collector_->set(rev_power_) ;
        }

        void CollectOffAction::run() 
        {
            Intake &intake = dynamic_cast<Intake &>(getSubsystem()) ; 
            MotorEncoderGoToAction::run() ;

            if (isDone())
                intake.collector_.get()->set(0.0) ;
        }

        void CollectOffAction::cancel() {
            Intake &intake = dynamic_cast<Intake &>(getSubsystem()) ;             
            MotorEncoderGoToAction::cancel() ;
            intake.collector_.get()->set(0.0) ;                  //collector "off"
            setDone() ;
        }
    }
}
