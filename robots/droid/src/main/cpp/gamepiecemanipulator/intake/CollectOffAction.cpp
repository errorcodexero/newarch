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
        CollectOffAction::CollectOffAction(Intake &subsystem) : xero::base::Action(subsystem.getRobot().getMessageLogger()), 
                                                              sequence_(subsystem.getRobot().getMessageLogger()),
                                                              sub_(subsystem) {
   
            collector_power_ = 0.0;    //collector "off""

            double pos ;
            ActionPtr act ;
     
            // Intake arm up //
            pos = subsystem.getRobot().getSettingsParser().getDouble("intake:arm:collectoff:pos") ;
            act = std::make_shared<MotorEncoderGoToAction>(sub_, pos) ;
            sequence_.pushAction(act) ;

            // hold arm up //
            act = std::make_shared<MotorEncoderHoldAction>(sub_, pos) ;
            sequence_.pushAction(act) ;
        }
        
        void CollectOffAction::start() {
            xero::base::Action::start();
            sequence_.start() ;
        }

        void CollectOffAction::run() {
            // set collector motors off //
            sub_.collector_.get()->set(collector_power_) ;                 //collector "off"

            xero::base::Action::run();
            // arm - up and hold sequence //
            sequence_.run() ;
            if (sequence_.isDone())
                setDone() ;
        }

        void CollectOffAction::cancel() {
            sub_.collector_.get()->set(collector_power_) ;                  //collector "off"
            
            xero::base::Action::cancel();
            sequence_.cancel() ;
            setDone() ;
        }

    }
}
