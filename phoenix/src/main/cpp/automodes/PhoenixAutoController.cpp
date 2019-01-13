#include "PhoenixAutoController.h"
#include "CrossTheLine.h"
#include "CenterSwitchLeft.h"
#include "CenterSwitchRight.h"
#include "FollowPathAutoMode.h"
#include <frc/DriverStation.h>

using namespace xero::base ;

namespace xero {
    namespace phoenix {
        PhoenixAutoController::PhoenixAutoController(Robot &robot) : AutoController(robot) {
        }
        
        PhoenixAutoController::~PhoenixAutoController() {
        }

        void PhoenixAutoController::updateAutoMode(int sel, const std::string &gamedata) {
            sel = 2 ;

            Robot &robot = getRobot() ;
            Phoenix &phoenix = dynamic_cast<Phoenix &>(robot) ;
            ActionSequencePtr mode = nullptr ;

            auto &ds = frc::DriverStation::GetInstance() ;
            std::string game = ds.GetGameSpecificMessage() ;
            char myswitch = game[0] ;

            switch(sel) {
            case 0:
                mode = std::make_shared<CrossTheLine>(phoenix) ;
                break ;

            case 1:
                if (myswitch == 'L')
                    mode = std::make_shared<CenterSwitchLeft>(phoenix) ;
                else
                    mode = std::make_shared<CenterSwitchRight>(phoenix) ;
                break ;

            case 2:
                mode = std::make_shared<FollowPathAutoMode>(phoenix) ;
                break ;
            }

            setAction(mode) ;
        }
    }
}
