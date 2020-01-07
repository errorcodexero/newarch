#include "DroidSubsystem.h"
#include "shooter/Shooter.h"
#include "collector/Collector.h"
#include "conveyor/Conveyor.h"
#include "controlpanelrotator/ControlPanelRotator.h"
#include "climber/climber.h"
#include "droidids.h"
#include <Robot.h>
#include <list>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace droid {
        DroidSubsystem::DroidSubsystem(Robot &robot) : RobotSubsystem(robot, "droid") {

            ////%% Start AddDriveBase
            addTankDrive() ;
            ////%% End AddDriveBase

            ////%% Start AddChildSubsystems
            oi_ = std::make_shared<DroidOISubsystem>(this) ;
            addChild(oi_) ;

            shooter_ = std::make_shared<Shooter>(this);
            addChild(shooter_);
            
            collector_ = std::make_shared<Collector>(this);
            addChild(collector_);

            conveyor_ = std::make_shared<Conveyor>(this);
            addChild(conveyor_);
            
            control_panel_rotator_ = std::make_shared<ControlPanelRotator>(this);
            addChild(control_panel_rotator_);

            climber_ = std::make_shared<Climber>(this);
            addChild(climber_);
            ////%% End AddChildSubsystems
        }

        DroidSubsystem::~DroidSubsystem() {
        }

        bool DroidSubsystem::canAcceptAction(ActionPtr act) {
            return false ;
        }
    }
}
