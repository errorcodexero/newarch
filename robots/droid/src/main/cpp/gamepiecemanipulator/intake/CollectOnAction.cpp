#include <MessageLogger.h>
#include <Robot.h>
#include <motorencodersubsystem/MotorEncoderGoToAction.h>
#include <motorencodersubsystem/MotorEncoderHoldAction.h>
#include <motors/MotorController.h>
#include <actions/Action.h>
#include "Subsystem.h"
#include "CollectOnAction.h"
#include "Intake.h"

namespace xero {
    namespace droid {
        std::string CollectOnAction::action_name("CollectOnAction");
        CollectOnAction::CollectOnAction(Intake &subsystem) : IntakeAction(subsystem),
                                                              sequence_(subsystem.getRobot().getMessageLogger())
        {
   
            collector_power_ = subsystem.getRobot().getSettingsParser().getDouble("intake:collector:motor:power") ;

            double pos ;
            ActionPtr act ;
     
            // Intake arm down //
            pos = subsystem.getRobot().getSettingsParser().getDouble("intake:arm:collecton:pos") ;
            act = std::make_shared<MotorEncoderGoToAction>(subsystem, pos) ;
            sequence_.pushAction(act) ;
        }
        
        void CollectOnAction::start() {
            xero::base::Action::start();
            sequence_.start() ;
            getSubsystem().collector_.get()->set(collector_power_) ;            
        }

        void CollectOnAction::run() {
            xero::base::Action::run();
            
            // arm - down and hold sequence //
            sequence_.run() ;
            if (sequence_.isDone())
                setDone() ;
        }

        void CollectOnAction::cancel() {
            getSubsystem().collector_.get()->set(0) ;                  //collector "off"
            
            xero::base::Action::cancel();
            sequence_.cancel() ;
            setDone() ;
        }

    }
}
