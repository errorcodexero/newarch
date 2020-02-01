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
        CollectOnAction::CollectOnAction(Intake &subsystem) : xero::base::MotorEncoderGoToAction(subsystem, "hw:collect:motor:on"), 
                                                              sequence_(subsystem.getRobot().getMessageLogger()) {
   
            collector_power_ = getSubsystem().getRobot().getSettingsParser().getDouble("intake:collector:motor:power") ;

            double pos ;
            ActionPtr act ;
     
            // Intake arm down //
            pos = subsystem.getRobot().getSettingsParser().getDouble("intake:arm:collect:pos") ;
            act = std::make_shared<MotorEncoderGoToAction>(getSubsystem(), pos) ;
            sequence_.pushAction(act) ;

            // hold arm down //
            act = std::make_shared<MotorEncoderHoldAction>(getSubsystem(), pos) ;
            sequence_.pushAction(act) ;
        }
        
        void CollectOnAction::start() {
            xero::base::MotorEncoderGoToAction::start();
            sequence_.start() ;
        }

        void CollectOnAction::run() {
            // set collector motors on //
            getSubsystem().collector_.get()->set(collector_power_) ;

            xero::base::MotorEncoderGoToAction::run();
            // arm - down and hold sequence //
            sequence_.run() ;
            if (sequence_.isDone())
                setDone() ;
        }

        void CollectOnAction::cancel() {
            xero::base::MotorEncoderGoToAction::cancel();
            sequence_.cancel() ;
            setDone() ;
        }

    }
}
