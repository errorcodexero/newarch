#include "DroidSubsystem.h"
#include "controlpanelrotator/ControlPanelRotator.h"
#include "gamepiecemanipulator/GamePieceManipulator.h"
#include "turret/Turret.h"
#include "climber/Climber.h"
#include "droidids.h"
#include <Robot.h>
#include <list>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace droid {
        DroidSubsystem::DroidSubsystem(Robot &robot) : RobotSubsystem(robot, "droid") {

            addTankDrive() ;

            oi_ = std::make_shared<DroidOISubsystem>(this) ;
            addChild(oi_) ;            

            control_panel_rotator_ = std::make_shared<ControlPanelRotator>(this);
            addChild(control_panel_rotator_);

            climber_ = std::make_shared<Climber>(this);
            addChild(climber_);

            manip_ = std::make_shared<GamePieceManipulator>(this);
            addChild(manip_);

            turret_ = std::make_shared<Turret>(this);
            addChild(turret_);
        }

        DroidSubsystem::~DroidSubsystem() {
        }

        bool DroidSubsystem::canAcceptAction(ActionPtr act) {
            return false ;
        }
    }
}
