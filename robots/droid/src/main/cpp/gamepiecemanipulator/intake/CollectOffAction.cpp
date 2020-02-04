#include <MessageLogger.h>
#include <Robot.h>
#include <motorencodersubsystem/MotorEncoderGoToAction.h>
#include <motorencodersubsystem/MotorEncoderHoldAction.h>
#include <motors/MotorController.h>
#include <actions/Action.h>
#include "Subsystem.h"
#include "CollectOffAction.h"
#include "Intake.h"

namespace xero {
    namespace droid {
        std::string CollectOffAction::action_name("CollectOffAction");
        CollectOffAction::CollectOffAction(Intake &subsystem) : IntakeAction(subsystem), 
                                                              sequence_(subsystem.getRobot().getMessageLogger())
        {
            collector_power_ = 0.0;    //collector "off""

            double pos ;
            ActionPtr act ;
     
            // Intake arm up //
            pos = subsystem.getRobot().getSettingsParser().getDouble("intake:arm:collectoff:pos") ;
            act = std::make_shared<MotorEncoderGoToAction>(getSubsystem(), pos) ;
            sequence_.pushAction(act) ;

            // hold arm up //
            act = std::make_shared<MotorEncoderHoldAction>(getSubsystem(), pos) ;
            sequence_.pushAction(act) ;
        }
        
        void CollectOffAction::start() {
            xero::base::Action::start();
            sequence_.start() ;
        }

        void CollectOffAction::run() {
            // set collector motors off //
            getSubsystem().collector_.get()->set(collector_power_) ;                 //collector "off"

            xero::base::Action::run();
            // arm - up and hold sequence //
            sequence_.run() ;
            if (sequence_.isDone())
                setDone() ;
        }

        void CollectOffAction::cancel() {
            getSubsystem().collector_.get()->set(collector_power_) ;                  //collector "off"
            
            xero::base::Action::cancel();
            sequence_.cancel() ;
            setDone() ;
        }

    }
}
