#include "ThreeCubeAutoMode.h"
#include "liftingcollector/LiftingCollector.h"
#include "Phoenix.h"
#include <actions/Action.h>
#include <actions/ParallelAction.h>
#include <actions/SequenceAction.h>
#include <actions/DelayAction.h>
#include <motorencodersubsystem/MotorEncoderGoToAction.h>
#include <singlemotorsubsystem/SingleMotorPowerAction.h>
#include <tankdrive/actions/TankDriveFollowPathAction.h>

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
            std::shared_ptr<ParallelAction> parallel = std::make_shared<ParallelAction>();
            std::shared_ptr<SequenceAction> sequence = std::make_shared<SequenceAction>(phoenix.getMessageLogger());

            act = std::make_shared<TankDriveFollowPathAction>(*tankdrive, "ThreeScale_P1");
            parallel->addSubActionPair(tankdrive, act);

            parallel->addAction(sequence);
            sequence->pushAction(std::make_shared<DelayAction>(1.0));
            sequence->pushSubActionPair(lifter, std::make_shared<MotorEncoderGoToAction>(*lifter, "lifter:height:scale"));

            pushAction(parallel);

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