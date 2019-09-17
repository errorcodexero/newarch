#include "automodes/$$RobotName$$AutoModeController.h"
#include "$$RobotName$$.h"
#include <tankdrive/modes/FollowPathAutomode.h>
#include <tankdrive/modes/ScrubCharMode.h>
#include <tankdrive/modes/StraightCharAutoMode.h>
#include <MessageLogger.h>
#include <frc/DriverStation.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace $$robotname$$ {
        $$RobotName$$AutoModeController::$$RobotName$$AutoModeController(Robot &robot) : AutoController(robot) {
        }
        
        $$RobotName$$AutoModeController::~$$RobotName$$AutoModeController() {
        }

        void $$RobotName$$AutoModeController::updateAutoMode(int sel, const std::string &gamedata) {
            AutoModePtr mode = nullptr ;

            switch(sel) {
            case 0:
                break ;

            case 1:
                break ;

            case 2:
                break ;

            case 3:
                break ;

            case 4:
                break ;

            case 5:
                break ;

            case 6:
                break ;

            case 7:
                mode = std::make_shared<StraightCharAutoMode>(getRobot(), 0.4, 10.0) ;
                break ;

            case 8:
                mode = std::make_shared<ScrubCharMode>(getRobot(), 0.4, 360.0 * 4.0) ;
                break ;

            case 9:
                // mode = std::make_shared<FollowPathAutomode>(getRobot(), "Tune_Tune") ;
                break ;
            }
            setAction(mode) ;
        }
    }
}
