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
        CollectOnAction::CollectOnAction(Intake &subsystem) : xero::base::Action(subsystem.getRobot().getMessageLogger()),
                                                              sequence_(subsystem.getRobot().getMessageLogger()),
                                                              sub_(subsystem) {
   
            collector_power_ = sub_.getRobot().getSettingsParser().getDouble("intake:collector:motor:power") ;

            double pos ;
            ActionPtr act ;
     
            // Intake arm down //
            pos = subsystem.getRobot().getSettingsParser().getDouble("intake:arm:collecton:pos") ;
            act = std::make_shared<MotorEncoderGoToAction>(sub_, pos) ;
            sequence_.pushAction(act) ;

            // hold arm down //
            act = std::make_shared<MotorEncoderHoldAction>(sub_, pos) ;
            sequence_.pushAction(act) ;
        }
        
        void CollectOnAction::start() {
            xero::base::Action::start();
            sequence_.start() ;
        }

        void CollectOnAction::run() {
            // set collector motors on //
            sub_.collector_.get()->set(collector_power_) ;

            xero::base::Action::run();
            // arm - down and hold sequence //
            sequence_.run() ;
            if (sequence_.isDone())
                setDone() ;
        }

        void CollectOnAction::cancel() {
            sub_.collector_.get()->set(0) ;                  //collector "off"
            
            xero::base::Action::cancel();
            sequence_.cancel() ;
            setDone() ;
        }

    }
}
