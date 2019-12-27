#include "ThreeCubeAutoMode.h"
#include "Phoenix.h"
#include <actions/Action.h>
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

            act = std::make_shared<TankDriveFollowPathAction>(*tankdrive, "ThreeScale_P1");
            pushSubActionPair(tankdrive, act);
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