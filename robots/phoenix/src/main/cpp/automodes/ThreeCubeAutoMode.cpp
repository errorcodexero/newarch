#include "ThreeCubeAutoMode.h"
#include "Phoenix.h"
#include "liftingcollector/LiftingCollector.h"
#include "phlifter/LifterBreakAction.h"
#include "phlifter/LifterShiftAction.h"

#include <actions/Action.h>
#include <actions/ParallelAction.h>
#include <actions/SequenceAction.h>
#include <actions/DelayAction.h>
#include <motorencodersubsystem/MotorEncoderGoToAction.h>
#include <singlemotorsubsystem/SingleMotorPowerAction.h>
#include <tankdrive/actions/TankDriveFollowPathAction.h>
#include <tankdrive/actions/TankDriveAngleAction.h>

using namespace xero::base;
using namespace xero::misc;

namespace xero
{
    namespace phoenix
    {
        std::string ThreeCubeAutoMode::left_name_("LeftThreeCubeScale");
        std::string ThreeCubeAutoMode::right_name_("RightThreeCubeScale");
        std::string ThreeCubeAutoMode::left_desc_("LeftThreeCubeScale");
        std::string ThreeCubeAutoMode::right_desc_("RightThreeCubeScale");

        ThreeCubeAutoMode::ThreeCubeAutoMode(xero::base::Robot &robot, ThreeCubeAutoMode::Side side)
            : PhoenixAutoMode(robot, genName(side), genDesc(side))
        {
            side_ = side;

            ActionPtr act ;
            auto &phoenix = dynamic_cast<Phoenix &>(getRobot()) ;
            auto tankdrive = phoenix.getPhoenixRobotSubsystem()->getTankDrive() ;
            auto lifter = phoenix.getPhoenixRobotSubsystem()->getLiftingCollector()->getLifter();
            auto grabber = phoenix.getPhoenixRobotSubsystem()->getLiftingCollector()->getCollector()->getGrabber();
            auto intake = phoenix.getPhoenixRobotSubsystem()->getLiftingCollector()->getCollector()->getIntake();
            std::shared_ptr<ParallelAction> parallel;

            std::shared_ptr<SequenceAction> sequence;


            // Parallel one - drive to scale while raising the lift
            parallel = std::make_shared<ParallelAction>();
            pushAction(parallel);

            // Parallel one action one - action to drive to scale
            act = std::make_shared<TankDriveFollowPathAction>(*tankdrive, "ThreeScale_P1");
            parallel->addSubActionPair(tankdrive, act);

            // Parallel one action two - sequence to raise lift to scale height
            sequence = std::make_shared<SequenceAction>(phoenix.getMessageLogger());
            parallel->addAction(sequence);
            sequence->pushAction(std::make_shared<DelayAction>(1.0));
            sequence->pushSubActionPair(lifter, std::make_shared<LifterShiftAction>(*lifter, true)) ;                          // Shift to high gear (make this false for low gear)
            sequence->pushSubActionPair(lifter, std::make_shared<LifterBreakAction>(*lifter, false)) ;                         // Disable the break             
            sequence->pushSubActionPair(lifter, std::make_shared<MotorEncoderGoToAction>(*lifter, "lifter:height:scale"));
            sequence->pushSubActionPair(lifter, std::make_shared<LifterBreakAction>(*lifter, true)) ;                         // Disable the break  

            // Action to eject cube
            act = std::make_shared<SingleMotorPowerAction>(*intake, "intake:eject:fast:power", "intake:eject:fast:duration");
            pushSubActionPair(intake, act);

            // Parallel two - action to lower lift to floor, rotate drivebase and drive to cube at corner of scale
            parallel = std::make_shared<ParallelAction>();
            pushAction(parallel);
            
            // Parllel two action one - sequence to lower lift to floor height
            sequence = std::make_shared<SequenceAction>(phoenix.getMessageLogger());
            parallel->addAction(sequence);            
            sequence->pushSubActionPair(lifter, std::make_shared<LifterBreakAction>(*lifter, false)) ;
            act = std::make_shared<MotorEncoderGoToAction>(*lifter, "lifter:height:floor");
            sequence->pushSubActionPair(lifter, act);
            sequence->pushSubActionPair(lifter, std::make_shared<LifterBreakAction>(*lifter, true)) ;                

            // Parallel two action two - sequence to rotate toward scale and drive to first cube
            sequence = std::make_shared<SequenceAction>(phoenix.getMessageLogger());
            parallel->addAction(sequence);            
            act = std::make_shared<TankDriveAngleAction>(*tankdrive, "automode:threecube:rotate1", true);
            sequence->pushSubActionPair(tankdrive, act);
            act = std::make_shared<TankDriveFollowPathAction>(*tankdrive, "ThreeScale_P2");
            sequence->pushSubActionPair(tankdrive, act);

            // Parallel three - action to rotate to scale and then drive to scale while raising lift
            parallel = std::make_shared<ParallelAction>();
            pushAction(parallel);

            // Parallel three action one - sequence to rotate to scale and drive to scale
            sequence = std::make_shared<SequenceAction>(phoenix.getMessageLogger());
            parallel->addAction(sequence); 

            act = std::make_shared<TankDriveAngleAction>(*tankdrive, "automode:threecube:rotate2", true);
            sequence->pushSubActionPair(tankdrive, act);
            act = std::make_shared<TankDriveFollowPathAction>(*tankdrive, "ThreeScale_P3");
            sequence->pushSubActionPair(tankdrive, act);
         
            // Parallel three action two - sequence to raise lift to scale height
            sequence = std::make_shared<SequenceAction>(phoenix.getMessageLogger());
            parallel->addAction(sequence);            
            sequence->pushSubActionPair(lifter, std::make_shared<LifterBreakAction>(*lifter, false)) ;
            act = std::make_shared<MotorEncoderGoToAction>(*lifter, "lifter:height:scale");
            sequence->pushSubActionPair(lifter, act);
            sequence->pushSubActionPair(lifter, std::make_shared<LifterBreakAction>(*lifter, true)) ;  

            // Action to eject cube
            act = std::make_shared<SingleMotorPowerAction>(*intake, "intake:eject:fast:power", "intake:eject:fast:duration");
            pushSubActionPair(intake, act);

            // Parallel four - rotate to scale and drive to second cube while lower lift to floor
            parallel = std::make_shared<ParallelAction>();
            pushAction(parallel);

            // Parallel four action one - sequence to lower lift to floor
            sequence = std::make_shared<SequenceAction>(phoenix.getMessageLogger());
            parallel->addAction(sequence);

            sequence->pushSubActionPair(lifter, std::make_shared<LifterBreakAction>(*lifter, false)) ;
            act = std::make_shared<MotorEncoderGoToAction>(*lifter, "lifter:height:floor");
            sequence->pushSubActionPair(lifter, act);
            sequence->pushSubActionPair(lifter, std::make_shared<LifterBreakAction>(*lifter, true)) ;                

            // Parallel four action two - sequence to rotate drivebase and drive to second cube
            sequence = std::make_shared<SequenceAction>(phoenix.getMessageLogger());
            parallel->addAction(sequence);

            act = std::make_shared<TankDriveAngleAction>(*tankdrive, "automode:threecube:rotate3", true);
            sequence->pushSubActionPair(tankdrive, act);
            act = std::make_shared<TankDriveFollowPathAction>(*tankdrive, "ThreeScale_P4");
            sequence->pushSubActionPair(tankdrive, act);

            // Parallel five - action to rotate to scale and then drive to scale while raising lift
            parallel = std::make_shared<ParallelAction>();
            pushAction(parallel);

            // Parallel three action one - sequence to rotate to scale and drive to scale
            sequence = std::make_shared<SequenceAction>(phoenix.getMessageLogger());
            parallel->addAction(sequence); 

            act = std::make_shared<TankDriveAngleAction>(*tankdrive, "automode:threecube:rotate4", true);
            sequence->pushSubActionPair(tankdrive, act);
            act = std::make_shared<TankDriveFollowPathAction>(*tankdrive, "ThreeScale_P5");
            sequence->pushSubActionPair(tankdrive, act);
         
            // Parallel three action two - sequence to raise lift to scale height
            sequence = std::make_shared<SequenceAction>(phoenix.getMessageLogger());
            parallel->addAction(sequence);            
            sequence->pushSubActionPair(lifter, std::make_shared<LifterBreakAction>(*lifter, false)) ;
            act = std::make_shared<MotorEncoderGoToAction>(*lifter, "lifter:height:scale");
            sequence->pushSubActionPair(lifter, act);
            sequence->pushSubActionPair(lifter, std::make_shared<LifterBreakAction>(*lifter, true)) ;  

            // Action to eject cube
            act = std::make_shared<SingleMotorPowerAction>(*intake, "intake:eject:fast:power", "intake:eject:fast:duration");
            pushSubActionPair(intake, act);            
        }

        ThreeCubeAutoMode::~ThreeCubeAutoMode()
        {
        }

        std::string ThreeCubeAutoMode::genName(ThreeCubeAutoMode::Side side)
        {
            if (side == Side::Left)
                return left_name_;

            return right_name_;
        }

        std::string ThreeCubeAutoMode::genDesc(ThreeCubeAutoMode::Side side)
        {
            if (side == Side::Left)
                return left_desc_;

            return right_desc_;            
        }
    }
}