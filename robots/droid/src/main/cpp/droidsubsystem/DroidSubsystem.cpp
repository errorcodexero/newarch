#include "DroidSubsystem.h"
#include "controlpanelrotator/ControlPanelRotator.h"
#include "gamepiecemanipulator/GamePieceManipulator.h"
#include "turret/Turret.h"
#include "climber/Climber.h"
#include "limelight/DroidLimeLight.h"
#include "targettracker/TargetTracker.h"
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

            alarms_ = std::make_shared<AlarmSubsystem>(this) ;
            addChild(alarms_) ;

#ifdef NOTYET
            control_panel_rotator_ = std::make_shared<ControlPanelRotator>(this);
            addChild(control_panel_rotator_);
#endif

            climber_ = std::make_shared<Climber>(this);
            addChild(climber_);

            manip_ = std::make_shared<GamePieceManipulator>(this);
            addChild(manip_);

            turret_ = std::make_shared<Turret>(this);
            addChild(turret_);

            limelight_ = std::make_shared<DroidLimeLight>(this);
            addChild(limelight_);

            target_tracker_ = std::make_shared<TargetTracker>(this);
            addChild(target_tracker_);
        }

        DroidSubsystem::~DroidSubsystem() {
        }

        bool DroidSubsystem::canAcceptAction(ActionPtr act) {
            return false ;
        }
    }
}
